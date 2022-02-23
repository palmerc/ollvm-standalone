//===- SplitBasicBlock.cpp - SplitBasicBlokc Obfuscation pass--------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the split basic block pass
//
//===----------------------------------------------------------------------===//

#include "Split.h"

#include <llvm/ADT/Statistic.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils/Local.h> // For DemoteRegToStack and DemotePHIToStack

#include "CryptoUtils.h"
#include "Utils.h"

#define DEBUG_TYPE "split"

using namespace llvm;
using namespace std;

// Stats
STATISTIC(Split, "Basicblock splitted");

static cl::opt<int> SplitNum("split_num", cl::init(2),
                             cl::desc("Split <split_num> time each BB"));

llvm::PassPluginLibraryInfo getFlatteningPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "split", "v1.0", [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [&](StringRef Name, FunctionPassManager &FPM,
                    ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "split") {
                    FPM.addPass(SplitBasicBlock());
                    return true;
                  }
                  return false;
                });
          }};
};

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getFlatteningPluginInfo();
}

char SplitBasicBlockLegacy::ID = 0;
RegisterPass<SplitBasicBlockLegacy>
    SplitLegacyX(/*PassArg=*/"legacy-split",
                 /*Name=*/"SplitBasicBlockLegacy",
                 /*CFGOnly=*/false,
                 /*is_analysis=*/false);

FunctionPass *createSplitBasicBlock() { return new SplitBasicBlockLegacy(); }

PreservedAnalyses SplitBasicBlock::run(Function &F,
                                       FunctionAnalysisManager &FAM) {
  bool changed = runOnFunction(F);
  return (changed ? PreservedAnalyses::none() : PreservedAnalyses::all());
}

bool SplitBasicBlock::runOnFunction(Function &F) {
  // Check if the number of applications is correct
  if (!((SplitNum > 1) && (SplitNum <= 10))) {
    errs() << "Split application basic block percentage\
            -split_num=x must be 1 < x <= 10";
    return false;
  }

  Function *tmp = &F;

  // Do we obfuscate
  split(tmp);
  ++Split;

  return false;
}

bool SplitBasicBlockLegacy::runOnFunction(Function &F) {
  return Impl.runOnFunction(F);
}

bool SplitBasicBlock::split(Function *f) {
  std::vector<BasicBlock *> origBB;
  int splitN = SplitNum;

  // Save all basic blocks
  for (Function::iterator I = f->begin(), IE = f->end(); I != IE; ++I) {
    origBB.push_back(&*I);
  }

  for (std::vector<BasicBlock *>::iterator I = origBB.begin(),
                                           IE = origBB.end();
       I != IE; ++I) {
    BasicBlock *curr = *I;

    // No need to split a 1 inst bb
    // Or ones containing a PHI node
    if (curr->size() < 2 || containsPHI(curr)) {
      continue;
    }

    // Check splitN and current BB size
    if ((size_t)splitN > curr->size()) {
      splitN = curr->size() - 1;
    }

    // Generate splits point
    std::vector<int> test;
    for (unsigned i = 1; i < curr->size(); ++i) {
      test.push_back(i);
    }

    // Shuffle
    if (test.size() != 1) {
      shuffle(test);
      std::sort(test.begin(), test.begin() + splitN);
    }

    // Split
    BasicBlock::iterator it = curr->begin();
    BasicBlock *toSplit = curr;
    int last = 0;
    for (int i = 0; i < splitN; ++i) {
      for (int j = 0; j < test[i] - last; ++j) {
        ++it;
      }
      last = test[i];
      if (toSplit->size() < 2)
        continue;
      toSplit = toSplit->splitBasicBlock(it, toSplit->getName() + ".split");
    }

    ++Split;
  }

  return Split > 0;
}

bool SplitBasicBlock::containsPHI(BasicBlock *b) {
  for (BasicBlock::iterator I = b->begin(), IE = b->end(); I != IE; ++I) {
    if (isa<PHINode>(I)) {
      return true;
    }
  }
  return false;
}

void SplitBasicBlock::shuffle(std::vector<int> &vec) {
  int n = vec.size();
  for (int i = n - 1; i > 0; --i) {
    std::swap(vec[i], vec[cryptoutils->get_uint32_t() % (i + 1)]);
  }
}

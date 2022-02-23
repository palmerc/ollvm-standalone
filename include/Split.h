//===- FlatteningIncludes.h - Flattening Obfuscation pass------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains includes and defines for the split basicblock pass
//
//===----------------------------------------------------------------------===//

#ifndef _SPLIT_INCLUDES_
#define _SPLIT_INCLUDES_

#include <llvm/IR/PassManager.h>


class SplitBasicBlock : public llvm::PassInfoMixin<SplitBasicBlock> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM);
  bool runOnFunction(llvm::Function &F);

protected:
  bool split(llvm::Function *f);
  bool containsPHI(llvm::BasicBlock *b);
  void shuffle(std::vector<int> &vec);
};

class SplitBasicBlockLegacy : public llvm::FunctionPass {
public:
  static char ID;
  SplitBasicBlockLegacy() : FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &F) override;

  SplitBasicBlock Impl;
};

llvm::FunctionPass *createSplitBasicBlock();
#endif

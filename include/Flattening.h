//===- FlatteningIncludes.h - Flattening Obfuscation pass------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains includes and defines for the flattening pass
//
//===----------------------------------------------------------------------===//

#ifndef _FLATTENING_INCLUDES_
#define _FLATTENING_INCLUDES_

#include <llvm/IR/PassManager.h>


using namespace std;

class Flattening : public llvm::PassInfoMixin<Flattening> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  bool runOnFunction(llvm::Function &F);

protected:
  bool flatten(llvm::Function *);
};

class FlatteningLegacy : public llvm::FunctionPass {
public:
  static char ID;
  FlatteningLegacy() : FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &F) override;

  Flattening Impl;
};

llvm::FunctionPass *createFlattening();

#endif

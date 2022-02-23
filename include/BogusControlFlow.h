//===- BogusControlFlow.h - BogusControlFlow Obfuscation
//pass-------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------------------===//
//
// This file contains includes and defines for the bogusControlFlow pass
//
//===--------------------------------------------------------------------------------===//

#ifndef _BOGUSCONTROLFLOW_H_
#define _BOGUSCONTROLFLOW_H_

#include <llvm/IR/PassManager.h>


class BogusControlFlow : public llvm::PassInfoMixin<BogusControlFlow> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  bool runOnFunction(llvm::Function &);
};

class BogusControlFlowLegacy : public llvm::FunctionPass {
public:
  static char ID;
  BogusControlFlowLegacy() : llvm::FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &) override;

  BogusControlFlow Impl;
};

llvm::FunctionPass *createBogus();

#endif

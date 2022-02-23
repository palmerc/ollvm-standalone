//===- SubstitutionIncludes.h - Substitution Obfuscation
//pass-------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains includes and defines for the substitution pass
//
//===----------------------------------------------------------------------===//

#ifndef _SUBSTITUTIONS_H_
#define _SUBSTITUTIONS_H_

#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>

#define NUMBER_ADD_SUBST 4
#define NUMBER_SUB_SUBST 3
#define NUMBER_AND_SUBST 2
#define NUMBER_OR_SUBST 2
#define NUMBER_XOR_SUBST 2

class Substitution : public llvm::PassInfoMixin<Substitution> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  bool runOnFunction(llvm::Function &F);

  Substitution() {
    funcAdd[0] = &Substitution::addNeg;
    funcAdd[1] = &Substitution::addDoubleNeg;
    funcAdd[2] = &Substitution::addRand;
    funcAdd[3] = &Substitution::addRand2;

    funcSub[0] = &Substitution::subNeg;
    funcSub[1] = &Substitution::subRand;
    funcSub[2] = &Substitution::subRand2;

    funcAnd[0] = &Substitution::andSubstitution;
    funcAnd[1] = &Substitution::andSubstitutionRand;

    funcOr[0] = &Substitution::orSubstitution;
    funcOr[1] = &Substitution::orSubstitutionRand;

    funcXor[0] = &Substitution::xorSubstitution;
    funcXor[1] = &Substitution::xorSubstitutionRand;
  }

protected:
  void (Substitution::*funcAdd[NUMBER_ADD_SUBST])(llvm::BinaryOperator *bo);
  void (Substitution::*funcSub[NUMBER_SUB_SUBST])(llvm::BinaryOperator *bo);
  void (Substitution::*funcAnd[NUMBER_AND_SUBST])(llvm::BinaryOperator *bo);
  void (Substitution::*funcOr[NUMBER_OR_SUBST])(llvm::BinaryOperator *bo);
  void (Substitution::*funcXor[NUMBER_XOR_SUBST])(llvm::BinaryOperator *bo);

  bool substitute(llvm::Function *);

  void addNeg(llvm::BinaryOperator *bo);
  void addDoubleNeg(llvm::BinaryOperator *bo);
  void addRand(llvm::BinaryOperator *bo);
  void addRand2(llvm::BinaryOperator *bo);

  void subNeg(llvm::BinaryOperator *bo);
  void subRand(llvm::BinaryOperator *bo);
  void subRand2(llvm::BinaryOperator *bo);

  void andSubstitution(llvm::BinaryOperator *bo);
  void andSubstitutionRand(llvm::BinaryOperator *bo);

  void orSubstitution(llvm::BinaryOperator *bo);
  void orSubstitutionRand(llvm::BinaryOperator *bo);

  void xorSubstitution(llvm::BinaryOperator *bo);
  void xorSubstitutionRand(llvm::BinaryOperator *bo);
};

class SubstitutionLegacy : public llvm::FunctionPass {
public:
  static char ID;
  SubstitutionLegacy() : FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &F) override;

  Substitution Impl;
};

llvm::FunctionPass *createSubstitution();

#endif

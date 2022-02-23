//===- SubstitutionIncludes.h - Substitution Obfuscation pass-------------------------===//
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


// LLVM include
#include <llvm/Pass.h>

// Namespace
using namespace llvm;
using namespace std;

namespace llvm {
	Pass *createSubstitution ();
	Pass *createSubstitution (bool flag);
}

#endif


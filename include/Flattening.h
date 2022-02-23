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


// LLVM include
#include <llvm/Pass.h>

// Namespace
using namespace std;

namespace llvm {
	Pass *createFlattening();
	Pass *createFlattening(bool flag);
}

#endif


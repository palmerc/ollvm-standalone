#include "Flattening.h"

#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

//===----------------------------------------------------------------------===//
// Command line options
//===----------------------------------------------------------------------===//
static cl::OptionCategory FlatteningCounterCategory{"split options"};

static cl::opt<std::string> InputModule{
    cl::Positional,
    cl::desc{"<Module to flatten basic blocks>"},
    cl::value_desc{"bitcode filename"},
    cl::init(""),
    cl::Required,
    cl::cat{FlatteningCounterCategory}};

//===----------------------------------------------------------------------===//
// static - implementation
//===----------------------------------------------------------------------===//
static void performFlattening(Module &M) {
  FunctionPassManager FPM;

  FPM.addPass(Flattening());

  FunctionAnalysisManager FAM;

  PassBuilder PB;
  PB.registerFunctionAnalyses(FAM);

  // Finally, run the passes registered with MPM
  for (auto &function : M.getFunctionList())
    FPM.run(function, FAM);
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
int main(int Argc, char **Argv) {
  // Hide all options apart from the ones specific to this tool
  cl::HideUnrelatedOptions(FlatteningCounterCategory);

  cl::ParseCommandLineOptions(Argc, Argv,
                              "Runs control-flow flattening on functions "
                              "calls in the input IR file\n");

  // Makes sure llvm_shutdown() is called (which cleans up LLVM objects)
  //  http://llvm.org/docs/ProgrammersManual.html#ending-execution-with-llvm-shutdown
  llvm_shutdown_obj SDO;

  // Parse the IR file passed on the command line.
  SMDiagnostic Err;
  LLVMContext Ctx;
  std::unique_ptr<Module> M = parseIRFile(InputModule.getValue(), Err, Ctx);

  if (!M) {
    errs() << "Error reading bitcode file: " << InputModule << "\n";
    Err.print(Argv[0], errs());
    return -1;
  }

  // Run the analysis and print the results
  performFlattening(*M);

  return 0;
}

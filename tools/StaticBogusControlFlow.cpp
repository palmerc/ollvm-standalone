#include "BogusControlFlow.h"

#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

static cl::OptionCategory BCFCounterCategory{"bcf options"};

static cl::opt<std::string> InputModule{cl::Positional,
                                        cl::desc{"<Module to bcf>"},
                                        cl::value_desc{"bitcode filename"},
                                        cl::init(""),
                                        cl::Required,
                                        cl::cat{BCFCounterCategory}};

static void performBCF(Module &M) {
  FunctionPassManager FPM;
  FPM.addPass(BogusControlFlow());

  FunctionAnalysisManager FAM;
  PassBuilder PB;
  PB.registerFunctionAnalyses(FAM);

  for (auto &function : M.getFunctionList())
    FPM.run(function, FAM);
}

int main(int Argc, char **Argv) {
  // Hide all options apart from the ones specific to this tool
  cl::HideUnrelatedOptions(BCFCounterCategory);

  cl::ParseCommandLineOptions(Argc, Argv,
                              "Runs bcf on functions in "
                              "the input IR file\n");

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
  performBCF(*M);

  return 0;
}

## OLLVM Standalone

A basic version of the [OLLVM project](https://github.com/obfuscator-llvm/obfuscator) removing all of the old LLVM code from the repo. You will want/need a debug version of LLVM 12 to compile against. See `build.sh` for example.

The static drivers take a .ll file as input.

### Emitting a .ll file
    SDK_PATH=$(xcrun -sdk macosx -show-sdk-path )
    clang++ -std=c++17 -isysroot ${SDK_PATH} -I${SDK_PATH}/usr/include -L${SDK_PATH}/usr/lib -S -emit-llvm -o output.ll input.cpp

### Using the .dylib with opt
    opt -S -stats -debug -load-pass-plugin libBogusControlFlow.dylib -passes=bcf -o output.ll input.ll

    echo "Generate bitcode"
    llvm-as -o output.bc input.ll

    echo "Create object file from bitcode"
    llc -filetype=obj input.bc

    echo "Produce executable"
    clang++ -isysroot ${SDK_PATH} -I${SDK_PATH}/usr/include -L${SDK_PATH}/usr/lib -o output input.o


Note: the code provided by the original project is for reference and has been altered as little as possible to get it to compile. It definitely has issues. For example the Bogus Control Flow code is implemented as a FunctionPass instead of a ModulePass, but modifies the Module. There are issues with exception handling. In general, don't use this for anything other than reference or demonstration purposes.


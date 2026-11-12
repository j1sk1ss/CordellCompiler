/* Main entry point of the compiler

Hello user, if you see this, you've came to the source code
of the Cordell Compiler. Actually - it's a good point where 
you can start, 'cause there you bascially entry to the pipeline.

How it works? The compiler has two parts at this point:
    - builder (manager)
    - pipeline (compiler itself)

The entry invokes the builder's entry point, which prepares
files that were provided to the compiler via arguments, and
then starts the compilation (optimisation and emmiting). */
#include <builder.h>

// !! DO NOT !! create any other entry point
//              or anything like this. This and only
//              this is the entry for the compiler.
int main(int argc, char* argv[]) {
    return BUILD_start(argc, argv);
}

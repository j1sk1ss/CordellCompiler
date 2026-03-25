# Language basics and logic
The **Cordell Programming Language (CPL)** is a system-level programming language designed for learning and experimenting with modern compiler concepts. It combines low-level capabilities from `ASM` with practices inspired by modern languages like `Rust` and `C`. `CPL` is intended for:
- **Radical simplicity** - let's make this language simple as it possible.
- **No safety** - if a programmer wants to destroy his PC, why we should stop him from doing this? If he doesn't care, why I should do?
- **System programming** — language designed primary for bootloaders (1-stage, 2-stage), edgeDBMSs, edgeFSs, primitive compilers, small interpreters. It is not for engines, web services, operating systems, games, etc. You may try to do some 'unsupported' stuff, but don't blame the language if it's impossible.
- **Experiments** - a compiler to test some techiques in real compilation without any overcomplexity. It has some artifacts from my experiments such as *symtable compression*, *perceptron inline*, *peephole DSL*, and *AST static analyzer*. You can find further information in the main README.
- **Educational purpose** — a language to study compiler design, interpreters, and programming language concepts. It isn't intent to serve as a regular language. You can try to use it, but again, it isn't his main purpose.

## Key Features
Basicly, there is nothing new was invented. This language mostly based on C language with some restrictions. But in a nutshell, there is a list of features:
- **Permissively statically-typed**: variables can't hold values of different types. P.S. The compiler attempts implicit conversions when assigning, that's why there is no *strong typing* in the language.
- **No structures**: language supports only primitive types as a part of an experiment. 
- **Deterministic control flow**: no hidden behaviors. All execution paths are explicit. The compiler can change IR, but can't change CFG (except optimizations).
- **Optimization**: input code can be optimized by techniques such as constant propagation and folding, linear invariant code motion, peephole optimization, function inline, tail recursion elimination, cold and hot branches.
- **Function overloading**: compiler supports the function overloading.
- **Default-args**: compiler supports default values in function arguments.
- **Headers**: cpl supports headers as it do C/++ languages.
- **Strings**: lagnuage supports and distinguishs strings from raw pointers.
- **Local functions**: functions can define local function as it can do functions in Rust.
- **Annotations**: language supports annotations as a solution for the restricted language's gramma.
- **Assembly**: Compiler generates an assembly file for the further compilation.
- **Separated Backend**: Compiler has a shared interface for different backends (ASMx86 (x16, x32, x64), RISC-V). This shared interface connects to the MidEnd and acts as a module.

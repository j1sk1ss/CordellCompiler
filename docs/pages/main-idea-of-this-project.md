# Main idea of this project

The main goal of the project is to study compiler architecture through a real, working compiler. CPL is deliberately small, low-level, and close to assembly/C-style programming, so most of the complexity lives in the compiler pipeline rather than in high-level language abstractions.

The language is also a test stand for optimization and analysis ideas: CFG construction, SSA, constant propagation, LICM, inlining, tail recursion elimination, register allocation, peephole optimization, and semantic analysis.

The long-term personal motivation is to use the compiler as part of the `CordellOS` ecosystem: write software for a custom OS using a custom compiler.

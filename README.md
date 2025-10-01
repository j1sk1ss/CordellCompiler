# Cordell Compiler
Cordell Compiler is a compact hobby compiler for Cordell Programming Language with a simple syntax, inspired by C and Rust. It is designed for studying compilation, code optimization, translation, and low-level microcode generation.

# Main idea of this project
Main goal of this project is learning of compilers architecture and porting one to CordellOS project (I want to code apps for OS inside this OS). Also, according to my bias to assembly and C languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison and etc).

# Usefull links and literature
- Aarne Ranta. *Implementing Programming Languages. An Introduction to Compilers and Interpreters*
- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools (Dragon Book)*
- Cytron et al. *Efficiently Computing Static Single Assignment Form and the Control Dependence Graph* (1991)
- Daniel Kusswurm. *Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition*

# Summary
## Navigation
## Intoduction
## EBNF
![EBNF](docs/EBNF.png)

## Sample code snippet
## Tokenization part
### Example of tokenized code
## Markup part
### Example of markup result
## AST part
### Example of AST
## HIR part
### Example of HIR
## CFG part
### Example of CFG
## Dominant calculation
### Dominant
### Strict dominance
### Dominance frontier
### Example of dominant frontier
## SSA form
### SSA idea
### Phi function
### Example of code
## Liveness analyzer part
### USE and DEF
### IN and OUT
### Point of deallocation
### Example code
## Register allocation part
### Graph coloring
## LIR (x86_64) part
## Codegen (nasm) part
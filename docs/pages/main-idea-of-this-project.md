# Main idea of this project
## Why I need a compiler?
The initial goal of this project (the compiler + language) was learning of compilers architecture and porting one to [CordellOS](https://github.com/j1sk1ss/CordellOS.PETPRJ) project (my initial idea was to have an opportunity to code programs for my own OS inside my own OS). </br>
Additionally, an optimizing compiler with a simple pipeline is a perfect ground for conducting any tests such as compressed symtables, DBMSs as symtables, perceptrons for inlines, switches and SMT solvers, DSL for peephole phases, etc.

## Why I need a language?
To support existed languages (such as C, Rust, Go, JS, Java etc.) I need to implement a complex parser which will consume a lot of development time. According to the main idea which is prioritizing the optimization over a syntax and parsing - support of an existing language is redundant. </br>
A simple language that behaves like a subset of C language with some things from Rust is a perfect solution of my problem. This new language can be treated as a language that somewhere is more advanced than C language, and somewhere is more restricted than C language. Justification of such restrictions (like intentionally removed strctures, enums and classes) is an interesting task as well. </br>
P.S.: *Also, according to my explicit bias to assembly and C languages, the new language is intended to remain as low-level as possible, but some features can be added in future with strings (inbuild concat, comparison and etc).*

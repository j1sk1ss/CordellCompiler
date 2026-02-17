# Cordell Compiler
Cordell Compiler is a compact hobby compiler for the `Cordell Programming Language` with a simple syntax, inspired by C and Rust. It is designed for studying compilation, code optimization, translation, and low-level microcode generation.
- *This README still in progress*

# Main idea of this project
Main goal of this project is learning of compilers architecture and porting one to the `CordellOS` project (I just want to code apps for `my` OS inside `my` OS). Also, given my bias to `assembly` and `C` languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison, etc).

# Used links and literature
- Aarne Ranta. *Implementing Programming Languages. An Introduction to Compilers and Interpreters*
- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools (Dragon Book)*
- Andrew W. Appel. *Modern Compiler Implementation in C (Tiger Book)*
- Cytron et al. *Efficiently Computing Static Single Assignment Form and the Control Dependence Graph* (1991)
- Daniel Kusswurm. *Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition*
- Jason Robert, Carey Patterson. *Basic Instruction Scheduling (and Software Pipelining)* (2001)

# Summary
This `README` file contains the main information about this compiler and the development approaches I’ve learnt while working on it. This repository also includes a `github.io` site with similar content and some interactive sections. For convenience, a `Navigation` block with quick links to the topics in this file is provided below:

- [Introduction](#introduction)
- [EBNF](#ebnf)
- [Code snippet](#sample-code-snippet)
- [PP part](#pp-part)
- [Tokenization part](#tokenization-part)
- [Markup part](#markup-part)
- [AST part](#ast-part)
   - [AST optimization](#ast-optimization)
- [HIR part](#hir-part)
- [CFG part](#cfg-part)
- [Dominant calculation](#dominant-calculation)
   - [Strict dominance](#strict-dominance)
   - [Dominance frontier](#dominance-frontier)
- [SSA form](#ssa-form)
   - [Phi function](#phi-function)
- [DAG part](#dag-part)
- [HIR optimization](#hir-optimization)
   - [Constant folding / propagation (First pass)](#constant-folding--propagation-first-pass)
   - [Dead Function Elimination (DFE)](#dead-function-elimination-dfe)
   - [Tail Recursion Elimination (TRE)](#tail-recursion-elimination-tre)
   - [Function inlining](#function-inlining)
   - [Loop canonicalization](#loop-canonicalization)
   - [Loop Invariant Code Motion (LICM)](#loop-invariant-code-motion-licm)
- [LIR part](#lir-part)
- [LIR instruction planning](#lir-instruction-planning)
- [LIR (x86_64) instruction selection](#lir-x86_64-instruction-selection)
- [LIR applying const propagation](#lir-applying-const-propagation)
- [LIR x86_64 example](#lir-x86_64-example)
- [Liveness analyzer part](#liveness-analyzer-part)
   - [USE and DEF](#use-and-def)
   - [IN and OUT](#in-and-out)
   - [Point of deallocation](#point-of-deallocation)
- [Register allocation part](#register-allocation-part)
   - [Graph coloring](#graph-coloring)
- [LIR peephole optimization](#lir-peephole-optimization)
   - [PTRN domain specific language](#ptrn-domain-specific-language)
   - [First pass](#first-pass)
   - [Second pass](#second-pass)
   - [Third pass](#third-pass)
- [Codegen (nasm) part](#codegen-nasm-part)
   - [Example of generated code](#example-of-generated-code)

## Project structure
- `examples/` - CordellLanguage code examples.
- `include/` - Include headers of this compiler and standart libs.
- `src/` - Source files of this compiler.
- `std/` - Standart libs that are used in this compiler.
- `tests/` - Tests folder for this complier.
- `vscode/` - VScode CordellLanguage extention.
- `commits.py` - Special commit tool for better maintaining this project.
- `Makefile` - Main build script.

## Introduction
This compiler isn't about a complex syntax and abstractions (It doesn't even support structures `yet`). Mainly I'm trying to implement modern approuches of code optimization and observe how an input code can be transformed to `better-performance` version of itself. This `README` is a description what I did. </br> 
Moreover I chose to take on an extra challenge — creating a programming language over supporting an existing one. This language has an `EBNF-defined` [[?]](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) syntax, its own [VS Code extension](https://github.com/j1sk1ss/CordellCompiler/tree/HIR_LIR_SSA/vscode), and small documentation. It won't be difficult given it's lack of complex structures such as `foreach` loops, `for` loops, `structures`, etc. </br>
While explaining each layer of the compiler, I will also provide direct examples written in this language.

## EBNF
Aforementioned language is called `Cordell Programming Language`. It's syntax mainly based on C language, but some keywords (such as `f64`, `f32`, `u64`, etc) [[?]](https://doc.rust-lang.org/book/ch03-02-data-types.html) came from Rust language, and some keywords (`ptr`) [[?]](https://teaching.idallen.com/dat2343/01f/notes/reserved_words.htm) came from Assembly. Below you can see an `EBNF` of this language.
<details>
<summary><strong>EBNF grammar</strong></summary>

![EBNF](docs/media/EBNF.jpg)
</details>

## Sample code snippet
The code below demonstrates the main capabilities of the `CPL` language, excluding already supported features such as `while` loops, `syscalls` and `strings` (we will talk about them later). </br>
Here we are implement the simple `strlen` function and test it on a string.

<details>
<summary><strong>Basic CPL code snippet</strong></summary>

```cpl
{
    function strlen(ptr i8 s) -> i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    start(i64 argc, ptr u64 argv) {
        str msg = "Hello world!";
        syscall(0x2000004, 1, ref msg, strlen(ref msg));
        exit 0;
    }
}
```
</details>

## PP part
This compiler has to parse not only a code, but also derictives. Derictives such as `include`, `define`, `undef`, `ifdef` and `ifndef` are supported by the preprocessor. These commands act similar to C/C++ derictives, except `define` that isn't able to work as a function. </br>
For instance:
```cpl
: string_h.cpl :
{
#ifndef STRING_H_
#define STRING_H_ 0
    : Get the size of the provided string
      Params
        - `s` - Input string.

      Returns the size (i64). :
    function strlen(ptr i8 s) -> i64;
#endif
}

: print_h.cpl :
{
#ifndef PRINT_H_
#define PRINT_H_ 0
    #include "string_h.cpl"
    : Basic print function that is based on
      a syscall invoke.
      Params
      - `msg` - Input message to print.
      
      Returns i0 aka nothing. :
    function print(ptr str msg) -> i0;
#endif
}

: basic.cpl :
{
    #include "print_h.cpl"
    start(i64 argc, ptr u64 argv) {
        print("Hello world!\n");
        exit 0;
    }
}
```

Will be converted into the code below:
```cpl
{
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/test_code/preproc/print_h.cpl"
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/test_code/preproc/string_h.cpl"

    function strlen(ptr i8 s) -> i64;

#line 4 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/test_code/preproc/print_h.cpl"

    function print(ptr str msg) -> i0;

#line 2 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/test_code/preproc/basic.cpl"

    start(i64 argc, ptr u64 argv) {
        print("Hello world!\n");
        exit 0;
    }
}
```

## Tokenization part
The tokenization part is responsible for splitting the input byte sequence (the result of the `fread` operation) into basic tokens. This module ignores all whitespace and separator symbols (such as `newlines` and `tabs`). It also classifies each token into one of the basic types: `number`, `string`, `delimiter`, `comma`, or `dot`.

<p align="center">
  <img src="docs/media/tokenization.png">
  <br>
  <em>Figure 1 — Basic token generation</em>
</p>

Code from the [Sample code snippet](#sample-code-snippet) section will produce the next list of tokens:
<details>
<summary><strong>List of tokens</strong></summary>

```
line=1, type=1, data=[{], 
line=1, type=2, data=[function], 
line=1, type=2, data=[strlen], 
line=1, type=1, data=[(], 
line=1, type=2, data=[ptr], 
line=1, type=2, data=[i8], 
line=1, type=2, data=[s], 
line=1, type=1, data=[)], 
line=1, type=0, data=[->], 
line=1, type=2, data=[i64], 
line=2, type=1, data=[{], 
line=2, type=2, data=[i64], 
line=2, type=2, data=[l], 
line=2, type=0, data=[=], 
glob line=2, type=3, data=[0], 
line=3, type=6, data=[;], 
line=3, type=2, data=[while], 
line=3, type=2, data=[dref], 
line=3, type=2, data=[s], 
line=3, type=6, data=[;], 
line=4, type=1, data=[{], 
line=4, type=2, data=[s], 
line=4, type=0, data=[+=], 
glob line=4, type=3, data=[1], 
line=5, type=6, data=[;], 
line=5, type=2, data=[l], 
line=5, type=0, data=[+=], 
glob line=5, type=3, data=[1], 
line=6, type=6, data=[;], 
line=7, type=1, data=[}], 
line=8, type=2, data=[return], 
line=8, type=2, data=[l], 
line=9, type=6, data=[;], 
line=10, type=1, data=[}], 
line=11, type=2, data=[start], 
line=11, type=1, data=[(], 
line=11, type=2, data=[i64], 
line=11, type=2, data=[argc], 
line=11, type=7, data=[,], 
line=11, type=2, data=[ptr], 
line=11, type=2, data=[u64], 
line=11, type=2, data=[argv], 
line=11, type=1, data=[)], 
line=12, type=1, data=[{], 
line=12, type=2, data=[str], 
line=12, type=2, data=[msg], 
line=12, type=0, data=[=], 
glob line=12, type=99, data=[Hello world!], 
line=13, type=6, data=[;], 
line=13, type=2, data=[syscall], 
line=13, type=1, data=[(], 
glob line=13, type=3, data=[0], 
line=13, type=7, data=[,], 
glob line=13, type=3, data=[1], 
line=13, type=7, data=[,], 
line=13, type=2, data=[ref], 
line=13, type=2, data=[msg], 
line=13, type=7, data=[,], 
line=13, type=2, data=[strlen], 
line=13, type=1, data=[(], 
line=13, type=2, data=[ref], 
line=13, type=2, data=[msg], 
line=13, type=1, data=[)], 
line=13, type=1, data=[)], 
line=14, type=6, data=[;], 
line=14, type=2, data=[exit], 
glob line=14, type=3, data=[0], 
line=15, type=6, data=[;], 
line=16, type=1, data=[}]
```
</details>

## Markup part
The markup stage is the second part of tokenization. Usually compilers don't distinguish `tokenizator` and `markuper`, but this compiler does. Markup stage operates only on the list of tokens from `tokenizator` including `scopes` support. </br>
The main idea is to perform basic semantic markup of variables. For instance, if we declare some `i32` variable which named `a` in a scope with `id=10`, all occurrences of `a` within the corresponding scope can be marked as having the `i32` type.

<p align="center">
  <img src="docs/media/markup.png">
  <br>
  <em>Figure 2 — Token markup</em>
</p>

List of tokens from the [Tokenization part](#tokenization-part) section will produce the next list of tokens:
<details>
<summary><strong>List of markuped tokens</strong></summary>

```
line=1, type=12, data=[{], 
line=1, type=55, data=[function], 
line=1, type=56, data=[strlen], 
line=1, type=10, data=[(], 
line=1, type=37, data=[i8], ptr 
line=1, type=92, data=[s], ptr 
line=1, type=11, data=[)], 
line=1, type=50, data=[->], 
line=1, type=34, data=[i64], 
line=2, type=12, data=[{], 
line=2, type=34, data=[i64], 
line=2, type=89, data=[l], 
line=2, type=73, data=[=], 
glob line=2, type=3, data=[0], 
line=3, type=6, data=[;], 
line=3, type=61, data=[while], 
line=3, type=92, data=[s], ptr dref 
line=3, type=6, data=[;], 
line=4, type=12, data=[{], 
line=4, type=92, data=[s], ptr 
line=4, type=69, data=[+=], 
glob line=4, type=3, data=[1], 
line=5, type=6, data=[;], 
line=5, type=89, data=[l], 
line=5, type=69, data=[+=], 
glob line=5, type=3, data=[1], 
line=6, type=6, data=[;], 
line=7, type=13, data=[}], 
line=8, type=48, data=[return], 
line=8, type=89, data=[l], 
line=9, type=6, data=[;], 
line=10, type=13, data=[}], 
line=11, type=47, data=[start], 
line=11, type=10, data=[(], 
line=11, type=34, data=[i64], 
line=11, type=89, data=[argc], 
line=11, type=7, data=[,], 
line=11, type=38, data=[u64], ptr 
line=11, type=93, data=[argv], ptr 
line=11, type=11, data=[)], 
line=12, type=12, data=[{], 
line=12, type=42, data=[str], 
line=12, type=97, data=[msg], 
line=12, type=73, data=[=], 
glob line=12, type=99, data=[Hello world!], 
line=13, type=6, data=[;], 
line=13, type=53, data=[syscall], 
line=13, type=10, data=[(], 
glob line=13, type=3, data=[0], 
line=13, type=7, data=[,], 
glob line=13, type=3, data=[1], 
line=13, type=7, data=[,], 
line=13, type=97, data=[msg], ref 
line=13, type=7, data=[,], 
line=13, type=57, data=[strlen], 
line=13, type=10, data=[(], 
line=13, type=97, data=[msg], ref 
line=13, type=11, data=[)], 
line=13, type=11, data=[)], 
line=14, type=6, data=[;], 
line=14, type=49, data=[exit], 
glob line=14, type=3, data=[0], 
line=15, type=6, data=[;], 
line=16, type=13, data=[}]
```
</details>

## AST part
Next, we need to parse this sequence of marked tokens to construct an `AST` (Abstract Syntax Tree). There are many approaches to achieve this — for example, `LL` parsing, `LR` parsing, or even `hybrid` techniques that combine `LL` and `LR`. A more complete list of parser types can be found [[here]](https://www.geeksforgeeks.org/compiler-design/types-of-parsers-in-compiler-design/) or in related compiler design books (see [Used links and literature](#used-links-and-literature) section).

<p align="center">
  <img src="docs/media/ast.png">
  <br>
  <em>Figure 3 — Basic AST generation</em>
</p>

AST that was generated from the [Markup part](#markup-part)'s list of markuped tokens:
<details>
<summary><strong>Cordell Compiler's AST dump</strong></summary>

```
{ scope, id=0 }
   { scope, id=1 }
      [function] (t=55, v_id=-1, s_id=0)
         [strlen] (t=56, v_id=0, s_id=0)
            [i64] (t=34, v_id=-1, s_id=0)
         { scope, id=2 }
            [i8] (t=37, ptr, v_id=-1, s_id=0)
               [s] (t=92, ptr, v_id=0, s_id=2)
            { scope, id=3 }
               [i64] (t=34, v_id=-1, s_id=0)
                  [l] (t=89, v_id=1, s_id=3)
                  [0] (t=3, v_id=-1, s_id=0, glob)
               [while] (t=61, v_id=0, s_id=3)
                  [s] (t=92, ptr, dref, v_id=0, s_id=2)
                  { scope, id=4 }
                     [+=] (t=69, v_id=-1, s_id=0)
                        [s] (t=92, ptr, v_id=0, s_id=2)
                        [1] (t=3, v_id=-1, s_id=0, glob)
                     [+=] (t=69, v_id=-1, s_id=0)
                        [l] (t=89, v_id=1, s_id=3)
                        [1] (t=3, v_id=-1, s_id=0, glob)
               [return] (t=48, v_id=0, s_id=3)
                  [l] (t=89, v_id=1, s_id=3)
      [start] (t=47, v_id=1, s_id=0)
         [i64] (t=34, v_id=-1, s_id=0)
            [argc] (t=89, v_id=2, s_id=1)
         [u64] (t=38, ptr, v_id=-1, s_id=0)
            [argv] (t=93, ptr, v_id=3, s_id=1)
         { scope, id=0 }
            { scope, id=5 }
               [str] (t=42, v_id=-1, s_id=0)
                  [msg] (t=97, v_id=4, s_id=5)
                  [Hello world!] (t=99, v_id=0, s_id=0, glob)
               [syscall] (t=53, v_id=-1, s_id=0)
                  [0] (t=3, v_id=-1, s_id=0, glob)
                  [1] (t=3, v_id=-1, s_id=0, glob)
                  [msg] (t=97, ref, v_id=4, s_id=5)
                  [strlen] (t=57, v_id=0, s_id=0)
                     [msg] (t=97, ref, v_id=4, s_id=5)
               [exit] (t=49, v_id=0, s_id=5)
                  [0] (t=3, v_id=-1, s_id=0, glob)
```
</details>

### AST optimization
When we have a correct `AST` representation of the input code, we can optionally perform some optimizations. We will not spend much time here and will only cover a few examples. Note that `AST-level` optimizations are mostly redundant in this project (they were very usefull when this Compiler didn't have an `IR` level).
- `Condition unrolling`. If we have an `if` statement with a constant condition, such as `if 1 { ... }`, or similar situation with a `while` keyword or a `switch` statement, we can unroll them by removing the condition and keeping only the body.
- `Dead scope elimination`. If a scope doesn't affect the environment, it can be safely removed.

## HIR part
`AST` representation of the input code must be flattened given future optimization phases. A common approach here is to convert an `AST` form into a `Three-Address Code` (3AC) [[?]](https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/13/Slides13.pdf). </br>
Three-Address Code implies that there is only three placeholders for addresses in each command. For example, the `a += b` command can be converted to `3AC` as `a = a + b`.

<p align="center">
  <img src="docs/media/HIR.png">
  <br>
  <em>Figure 4 — AST to HIR</em>
</p>

HIR that was obtained from the [AST part](#ast-part)'s structure transformation:
<details>
<summary><strong>Cordell Compiler's HIR dump</strong></summary>

```
{
    {
        fn strlen(i8* s) -> i64
        {
            u64s %0 = alloc(8);
            u64s %0 = load_arg();
            {
                i64s %1 = alloc(8);
                i64s %1 = num?: 0;
                lb11:
                u8t %5 = *(u64s %0);
                if u8t %5, goto lb12, else goto lb13;
                lb12:
                {
                    u64t %7 = num?: 1 as u64;
                    u64t %6 = u64s %0 + u64t %7;
                    u64s %0 = u64t %6;
                    i64t %8 = i64s %1 + num?: 1;
                    i64s %1 = i64t %8;
                }
                goto lb11;
                lb13:
                return i64s %1;
            }
        }
        
        start {
            {
                i64s %2 = alloc(8);
                i64s %2 = load_starg();
                u64s %3 = alloc(8);
                u64s %3 = load_starg();
                {
                    {
                        strs %4 = alloc(Hello world!);
                        use num?: 0;
                        use num?: 1;
                        u64t %9 = &(strs %4);
                        use u64t %9;
                        u64t %10 = &(strs %4);
                        use u64t %10;
                        i64t %11 = call strlen(i8* s) -> i64, argc args: u64t %10 ;
                        use i64t %11;
                        syscall, argc: args: num?: 0 num?: 1 u64t %9 i64t %11 ;
                        exit num?: 0;
                    }
                }
            }
        }
    }
}
```
</details>

## CFG part
With the `3AC` form of the input code, we can move on to `CFG` (Control Flow Graph) [[?]](https://en.wikipedia.org/wiki/Control-flow_graph) [[?]](https://www.geeksforgeeks.org/software-engineering/software-engineering-control-flow-graph-cfg/) creation. There are several ways to split `3AC` into basic blocks. One approach is using `leaders`, while another is to create a block for every command. The second approach is straightforward — each `3AC` instruction becomes its own block. The `leaders` approach, described in the *Dragon Book*, defines three rules for identifying the start of a block:

- The first instruction in a function.
- The target of a JMP instruction.
- The instruction immediately following a JMP.

In this compiler, both approaches are implemented, but for the example, we will use the approach of creating a block for every command. However, further optimizations will consume CFG based on `leaders` approuch.

<p align="center">
  <img src="docs/media/CFG.png">
  <br>
  <em>Figure 5 — CFG from HIR</em>
</p>

<details>
<summary><strong>Cordell Compiler's example CFG dump</strong></summary>

![markup](docs/media/CFG_example.png)
</details>

## Dominant calculation
With the structured `CFG`, we can move on to a `SSA` form [[?]](https://en.wikipedia.org/wiki/Static_single-assignment_form) [[?]](https://www.geeksforgeeks.org/compiler-design/static-single-assignment-with-relevant-examples/) [[?]](https://dl.acm.org/doi/10.1145/75277.75280). First of all, we need to calculate dominators [[?]](https://en.wikipedia.org/wiki/Dominator_(graph_theory)) for each block in the `CFG`. In the nutshell, a dominator of a block `Y` is a block `X` that appears on every path from the start block to `Y`. For example, the Figure 6 illustrates how this works.

<p align="center">
  <img src="docs/media/dominators.png">
  <br>
  <em>Figure 6 — How we find a dominator</em>
</p>

### Strict dominance
Now we need to find a strict dominator for every block in the `CFG`. The reason why we need to do this, is a placement of `phi` functions. We will talk about them later. </br>
Strict dominance tells us which block strictly dominates another. A block `X` strictly dominates block `Y` if `X` dominates `Y` (important note here: `X != Y`). Why do we need this? The basic dominance relation marks all blocks that dominate a given block, but later analyses often require only the closest one. A block `X` is said to be the strict dominator of `Y` if there is no other block `Z` such that `Z` strictly dominates `Y` and is itself strictly dominated by `X`. </br>
For example, Figure 7 illustrates how strict dominators are look like.

<p align="center">
  <img src="docs/media/strict_dominance.png">
  <br>
  <em>Figure 7 — How we find a strict dominator</em>
</p>

### Dominance frontier
The dominance frontier [[?]](https://pages.cs.wisc.edu/~fischer/cs701.f05/lectures/Lecture22.pdf) [[?]](https://stackoverflow.com/questions/69794988/how-to-build-dominance-frontier-for-control-flow-graph) of a block `X` is the set of blocks where the dominance of `X` ends. More precisely, it represents all the blocks that are partially influenced by `X`: `X` dominates at least one of their predecessors, but does not dominate the block itself. In other words, it marks the boundary where control flow paths from inside `X`'s dominance region meet paths coming from outside (Figure 8).

<p align="center">
  <img src="docs/media/dominance_frontier.png">
  <br>
  <em>Figure 8 — Find dominance frontier</em>
</p>

## SSA form
The `SSA` form performs renaming all re-assigned variables so that each assignment creates a new, unique variable. Also, modern `SSA` form is more complex then I've mentioned. They don't constrait itselves with only variables renaming for every assignment. For example, some `SSA` forms are able to handle working with arrays (indexies) [[?]](https://dl.acm.org/doi/10.1145/268946.268956). </bt>
At this moment, CordellCompiler supports the most "basic" `SSA` form, that is presented in Figure 9.

<p align="center">
  <img src="docs/media/ssa_basic.png">
  <br>
  <em>Figure 9 — "Basic" SSA form</em>
</p>

### Phi function
But if we encounter an `if` statement? I'm trying to ask, what we must do, when we aren't able to say wich uniqe variable should be used in `read` operation after constrol-flow statement? Let's consider the next example in Figure 10.

<p align="center">
  <img src="docs/media/ssa_problem.png">
  <br>
  <em>Figure 10 — The "phi" problem</em>
</p>

Which version of the variable `a` should be used in the declaration of `b` variable? The answer is simple — they `both`. Here’s the twist: in the `SSA` form, we can use a `φ (phi)` function, which tells the compiler which variable version to use. An example of a `φ` function is shown in Figure 11.

<p align="center">
  <img src="docs/media/phi_function.png">
  <br>
  <em>Figure 11 — Phi function</em>
</p>

But how do we determine where to place this function? Here, we use previously computed dominance information. We traverse the entire symbol table of variables. For each variable, we collect the set of blocks where it is defined (either declared or assigned). Then, for each block with a definition, we take its dominance frontier blocks and insert a `φ` function there (Figure 12).

<p align="center">
  <img src="docs/media/phi_placement.png">
  <br>
  <em>Figure 12 — Phi function placement</em>
</p>

Then, during the SSA renaming process, we keep track of each block that passes through a `φ`-function block, recording the version of the variable and the block number. This completes the SSA renaming phase, producing the following result in Figure 13.

<p align="center">
  <img src="docs/media/phi_final.png">
  <br>
  <em>Figure 13 — Final phi function</em>
</p>

## DAG part
With the complete `SSA` form, we can move on to the first optional optimizations. The first one requires building a `DAG` (Directed Acyclic Graph) [[?]](https://www.geeksforgeeks.org/compiler-design/directed-acyclic-graph-in-compiler-design-with-examples/) [[?]](https://en.wikipedia.org/wiki/Directed_acyclic_graph) representation of the code. In short, `DAG` shows how every value in the program is derived / how each variable obtains its value (with some exceptions for `arrays` and `strings`). Basic example is provided in Figure 14.

<p align="center">
  <img src="docs/media/base_DAG.png">
  <br>
  <em>Figure 14 — DAG</em>
</p>

Then, when we build the "basic" `DAG`, we check and merge all nodes that share the same hash (computed as a hash of their child nodes). If the nodes are identical and the base node is located in a dominating block, we can safely merge them.
![opt_dag](docs/media/opt_DAG.png)

The result of using the DAG is optimized code with Common Subexpression Elimination applied.
![res_dag](docs/media/res_DAG.png)

<details>
<summary><strong>DAG application in HIR optimization</strong></summary>

```cpl
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        ptr i32 b = ref a;
        dref b = 11;
        i32 c = 10;
        exit c;
    }
}
```
->
```
{
    start {
        {
            i64s %0 = alloc(8);
            i64s %0 = load_starg();
            u64s %1 = alloc(8);
            u64s %1 = load_starg();
            {
                i32s %2 = alloc(8);
                i32t %5 = num?: 10 as i32;
                i32s %2 = i32t %5;
                u64s %3 = alloc(8);
                u64t %6 = &(i32s %2);
                u64s %3 = u64t %6;
                u64t %7 = num?: 11 as u64;
                *(u64s %3) = u64t %7;
                i32s %4 = alloc(8);
                i32t %8 = num?: 10 as i32;
                i32s %4 = i32t %8;
                exit i32s %4;
            }
        }
    }
}
```
</details>

## HIR optimization
Before we going any further, we should optimize our HIR with avaliable meta-information from this level. The simplest optimization here is the `constant fold` optimization due to availability of `DAG`. Same situation with `DFE` optimization. Let's speak about this approaches. 

### Constant folding / propagation (First pass)
With formed `DAG` we can tell wich value is assigned to each variable. We don't transform code at this stage, we only define variable values in symtable. Also, we track arithmetics, that's why we can perform simple operations with already defined variables from symtable.
![hir_constfold](docs/media/HIR_constfold.png)

### Dead Function Elimination (DFE)
Dead function elimination, similar to `HIR` constant folding, won't transform source code. Instead of transformation, this optimization will mark all unused functions as unused. This approuch based on `Call Graph`, that can be seen below.
![hir_callgraph](docs/media/CallGraph.png)

### Tail Recursion Elimination (TRE)
Tail recursion elimination (based on CFG) find all functions where happens self-invoking at the end. The simplest example here is below:
```cpl
function foo(i32 a = 10) -> i8 {
   if a > 20; { return a; }
   return foo(a + 1);
}
```

When we found such function, we determine if it ready for `TRE`. Then we transform it into the cycle:
```cpl
function foo(i32 a = 10) -> i8 {
lbX:
   if a > 20; { return a; }
   a += 1;
goto lbX;
}
```

This optimization save us from stackframe allocation that has high price, especially if recursion occurs frequently.

### Function inlining
The function inlining happens, when function call gets 3 or more euristic score. What the euristic score? Each function call evaluated with the next params:
- Is function call in cycle? `+2`
- One from the following:
   - Is function size (in BaseBlocks) lower then 2? `+3`
   - Is function size (in BaseBlocks) lower then 5? `+2`
   - Is function size (in BaseBlocks) lower then 10? `+1`
   - Is function size (in BaseBlocks) larger then 15? `-3`

When function call marked as `inline candidate`, compiler simply copy all contents from function body, replace argument assign and return:
![func_inline](docs/media/inline.png)

### Loop canonicalization
Loop canonicalization is the important step before the LICM optimization. The main idea of this stage - is to create one entry and exit point in the loop. Let's consider next `CFG`: 
![CFG_before_canon](docs/media/CFG_loop_before_canon.png)

Main problem here, that we have `critical edge` from `BB1` to `BB3`, that permits us from motion some redundant code from loop. To solve it, we can simply create a `preheader` base block:
![CFG_loop_preheader](docs/media/CFG_loop_preheader.png)

### Loop Invariant Code Motion (LICM)
Which `HIR` operations we can safely move from loop?
- Those which didn't use inductive variables
- Those which didn't use loop variables

## LIR part
In the same way as during `HIR` generation, we now produce an intermediate representation similar to `3AC` — but using only two addresses. This step is relatively straightforward, as it primarily involves adapting instructions to the target machine’s addressing model. Because the exact implementation depends heavily on the target architecture (register count, instruction set, addressing modes, etc.), we typically don’t spend much time optimizing or generalizing this layer. Its main goal is simply to bridge the high-level `HIR` representation and the target-specific assembly form, ensuring that each instruction can be directly translated to a valid machine instruction.
![lir_gen](docs/media/lir_gen.png)

## LIR instruction planning
## LIR (x86_64) instruction selection
## LIR applying const propagation

## Liveness analyzer part
Several optimization techniques are based on data-flow analysis. Data-flow analysis itself relies on liveness analysis, which in turn depends on the program’s `SSA` form and control-flow graph (CFG). Now that we have established these fundamental representations, we can proceed with the `USE–DEF–IN–OUT` computation process.

### USE and DEF
`USE` and `DEF` are two sets associated with every `CFG` block. These sets represent all definitions and usages of variables within the block (recall that the code is already in `SSA` form). In short:
- `DEF` contains all variables that are written (i.e., assigned a new value).
- `USE` contains all variables that are read (i.e., their value is used).
![use_def](docs/media/use_def.png)

### IN and OUT
`IN` and `OUT` is a little bit complex part here. 
```
OUT[B] = union(IN[S])
IN[B]  = union(USE[B], OUT[B] − DEF[B])
```

First of all, to make the calculation much faster, we should traverse our list of `CFG` blocks in reverse order, computing `IN` and `OUT` for each block using the formulas above, and repeat this process until it stabilizes. Stabilization occurs when the previous sets (`primeIN` and `primeOUT`) are equal to the current sets (`currIN` and `currOUT`). This means that for every block we should maintain four sets:

- primeIN
- currIN
- primeOUT
- currOUT

After each iteration, the current values are copied into the corresponding prime sets, preparing them for the next comparison cycle.
![in_out](docs/media/in_out.png)

### Point of deallocation
At this point, we can determine where each variable dies. If a variable appears in the `IN` or `DEF` set but is not present in the `OUT` set, it means the variable is no longer used after this block, and we can safely insert a special `kill` instruction to mark it as dead. However, an important detail arises when dealing with pointer types. To handle them correctly, we construct a special structure called an `aliasmap`, which tracks ownership relationships between variables. This map records which variable owns another — meaning that one variable’s lifetime depends on another’s. For example, in code like this:
```cpl
{
   i32 a0 = 10;
   ptr i32 b0 = ref a0;
   dref b0 = 20;
}
```
the variable `a` is owned by `b`, so we must not kill `a` while `b` is still alive. In other words, the liveness of `a` depends on the liveness of `b`, and this dependency is preserved through the aliasmap.
![kill_var](docs/media/kill_var.png)

## Register allocation part
Now that we have the `IN`, `OUT`, `DEF`, and `USE` sets, we can construct an interference graph. The idea is straightforward: we create a vertex for each variable in the symbol table, and then, for every `CFG` block, we connect (i.e., add an edge between) each variable from the block’s `DEF` set with every variable from its `OUT` set. This connection represents that these two variables are live at the same time. The resulting structure is the interference graph, where:
- Vertices represent program variables.
- Edges represent liveness conflicts (interference) between variables.
![ig](docs/media/not_colored_ig.png)

### Graph coloring
Now we can determine which variables can share the same register using graph coloring. The solution to this problem is purely mathematical, and there are many possible strategies to color a graph. In short, the goal is to assign a color to every node (variable) in such a way that no two connected nodes share the same color. The output of this algorithm is a colored interference graph, where each color represents a distinct physical register, and all variables with the same color can safely reuse the same register without overlapping lifetimes.
![colored_ig](docs/media/colored_ig.png)

## LIR peephole optimization
Peephole optimization [[?]](https://www.geeksforgeeks.org/compiler-design/peephole-optimization-in-compiler-design/) is the one of the easiest and the one of the impactful optimizations (At least in this compiler). In a nutshell, this optimization simply does a pattern matching and replacing similar it does a regular expression. It finds patterns in an assembly code / a LIR representation that are inefficient and replaces it with efficient synonyms. </br>
The simplest example here, is the replacing of one inefficient command with an efficient one:
```asm
; mox rax, 0
xor rax, rax
```

### PTRN domain specific language
As it was mentioned before, the peephole optimization mostly is based on patterns. The problem here that there is a lot of existed patterns for optimization of Assembly language. It would be really inconvenient to try implement these patterns in C language by hand (regarding the necessity of the CordellCompiler's LIR support). </br>
To solve this problem, we can use the DSL language that produces such optimizers for LIR representation. To see more information about this DSL, see [this](src/lir/peephole/pattern_generator/README.md) README file.

### First pass
The first pass of the peeophole optimizator is a pattern matcher pass. This pass involves the aforementioned before generated pattern matcher function.

### Second pass
The second pass propogates values to destroy a complex sequence of redundant mov operations. Here I'm implying the next sequence of commands as a complex sequence of redundant movs:
```asm
mov rax, rbx
mov rdx, rax
mov rcx, rdx
mov r10, rcx
mov r11, r10
mov rax, r10
``` 

Such a bizarre code is produced after HIR and LIR optimizations. After the second pass, the code above is transformed into the code below:
```asm
mov rax, rbx
mov rdx, rbx
mov rcx, rbx
mov r10, rbx
mov r11, rbx
mov rax, rbx
```

### Third pass
Now, when we've obtained the optimized version of a code, we need to clean it. To do this task, we can simply use CFG and check, if the considering register in current line (For instance let's take the `mov rax, rbx` line) is used in a read operation, and if it used, skip it. Otherwise, if it somewhere is used in a write operation (before any read operation) we can safely drop this line. </br>
As we can see, the `rax` register, after the second pass, is only used in the write operation at the end of the code snippet. It signals, that we can safely drop this line.
```asm
; mov rax, rbx [dropped]
mov rdx, rbx
mov rcx, rbx
mov r10, rbx
mov r11, rbx
mov rax, rbx
```

Additionally, if a register doesn't affect on a program environment (e.g. isn't used in a syscall, function call, etc), it also can be safely marked as `dropped`:
```asm
; mov rax, rbx [dropped]
; mov rdx, rbx [dropped]
; mov rcx, rbx [dropped]
; mov r10, rbx [dropped]
; mov r11, rbx [dropped]
; mov rax, rbx [dropped]
```

P.S. This is a pretty synthetic example, thought.

<details>
<summary><strong>From HIR to LIR example</strong></summary>

From the HIR we can produce a high level of the LIR:
```
fn strlen(i8* s) -> i64
{
    %12 = ldparam();
    {
        kill(cnst: 0);
        kill(cnst: 1);
        %13 = num: 0;
        %14 = %13;
        %8 = num: 1 as u64;
        lb10:
        %6 = *(%15);
        cmp %6, cnst: 0;
        jne lb11;
        je lb12;
        lb11:
        {
            %7 = %15 + %8;
            %16 = %7;
            %9 = %14 + num: 1;
            %17 = %9;
        }
        %14 = %17;
        %15 = %16;
        jmp lb10;
        lb12:
        return %14;
    }
}
kill(cnst: 14);

start {
    {
        %18 = strt_loadarg();
        %19 = strt_loadarg();
        {
            %4 = str_alloc(str(Hello world!));
            %5 = arr_alloc(X);
            %10 = &(%5);
            kill(cnst: 3);
            kill(cnst: 18);
            kill(cnst: 4);
            kill(cnst: 19);
            kill(cnst: 10);
            kill(cnst: 5);
            kill(cnst: 2);
            stparam(%10);
            call strlen(i8* s) -> i64;
            %11 = fret();
            exit %11;
        }
    }
    kill(cnst: 11);
}
```
</details>

## LIR x86_64 instruction selection
Next step is LIR lowering. The most common way here - instruction selection. This is the first machine-depended step in compiler, that's why here we have some abstractions and implementations of different asm dialetcs (e.g., nasm x86_64 gnu, at&at x86_64 gnu, etc.).

<details>
<summary><strong>LIR selected instructions</strong></summary>

```
fn strlen(i8* s) -> i64
{
    {
        r11 = num: 0;
        r9 = r11;
        rdi = rdi;
        rbx = num: 1 as u64;
        lb10:
        r8 = *(rsi);
        cmp r8, cnst: 0;
        je lb12;
        jne lb11;
        lb11:
        {
            rax = rsi;
            rax = rax + rbx;
            rax = rax;
            rdx = rax;
            rsi = rdx;
            rax = r9;
            rax = rax + rbx;
            r10 = rax;
            rcx = r10;
            r9 = rcx;
            rbx = num: 1;
            rbx = rbx;
        }
        jmp lb10;
        lb12:
        return r9;
    }
}

start {
    {
        rbx = [rbp + 16];
        rax = rax;
        rbx = [rbp + 8];
        [rbp - 16] = num: 0;
        [rbp - 15] = num: 1;
        rdx = &([rbp - 16]);
        rdi = rdx;
        {
            [rbp - 29] = cnst: 72;
            [rbp - 28] = cnst: 101;
            [rbp - 27] = cnst: 108;
            [rbp - 26] = cnst: 108;
            [rbp - 25] = cnst: 111;
            [rbp - 24] = cnst: 32;
            [rbp - 23] = cnst: 119;
            [rbp - 22] = cnst: 111;
            [rbp - 21] = cnst: 114;
            [rbp - 20] = cnst: 108;
            [rbp - 19] = cnst: 100;
            [rbp - 18] = cnst: 33;
            [rbp - 17] = cnst: 0;
            kill(cnst: 4);
            kill(cnst: 5);
            call strlen(i8* s) -> i64;
            exit rax;
        }
    }
}
```

</details>

Maybe you have notice, that we also apply register allocation here. The reason why we wait till this stage, is `pre-coloring`. Main idea, that we precolor some variables with already known registers like `rax` and `rbx` in arithmetics, `rdi`, `rsi`... in ABI function call etc.

## Codegen (nasm) part
After completing the full code transformation pipeline, we can safely convert our `LIR` form into the `ASM` form, with a few small tricks applied during the unwrap process of special `LIR` instructions such as `EXITOP`, `STRT`, and others.
![lir2asm](docs/media/LIR_to_ASM.png)

<details>
<summary><strong>Final ASM</strong></summary>

```
; Compiled .asm file. File generated by CordellCompiler!
section .data
section .rodata
section .bss
section .text
global _main

; BB20: 
_cpl_strlen:
push rbp
mov rbp, rsp
mov rcx, rdi
mov rsp, rdi
xor rbx, rbx
mov rbp, rbx

; BB27: 
mov rdi, rsp
mov rbx, rsp
mov rsi, rbp
mov rcx, rbp
mov rdx, 1

; BB21: 
lb11:
mov r11, rbx
mov r10b, [r11]
cmp r10b, 0
je lb13
jne lb12

; BB22: 
lb12:
mov r12, rbx
mov rax, rbx
add rax, rdx
mov rsi, rax
mov rbp, rax
mov r8, rax
mov r14, rax
mov rax, r14
shr rax, rax
mov r13, rax
mov rsp, rax
mov rax, rsp
mov r15, rsp
mov rcx, rsp
mov r9, rsp
mov rbx, rsp
jmp lb11

; BB23: 
lb13:
mov rdi, rcx
mov rax, rcx
mov rsp, rbp
pop rbp
ret

; BB24:

; BB25: 
_main:
push rbp
mov rbp, rsp
sub rsp, 16
mov byte [rbp - 13], 72
mov byte [rbp - 12], 101
mov byte [rbp - 11], 108
mov byte [rbp - 10], 108
mov byte [rbp - 9], 111
mov byte [rbp - 8], 32
mov byte [rbp - 7], 119
mov byte [rbp - 6], 111
mov byte [rbp - 5], 114
mov byte [rbp - 4], 108
mov byte [rbp - 3], 100
mov byte [rbp - 2], 33
mov byte [rbp - 1], 0
lea rax, qword [rbp - 13]
mov r11, rax
mov r15, qword [rbp + 16]
mov r8, qword [rbp + 8]
mov rbx, qword [rbp + 8]
mov r9, r11
mov rdi, r9
push rax
push rdx
push rsp
push r14
push r10
push rbp
push r11
push r12
push rax
call _cpl_strlen
pop rax
pop r12
pop r11
pop rbp
pop r10
pop r14
pop rsp
pop rdx
pop rax
mov r13, rax
mov rsp, rax
xor rbp, rbp
mov rax, rbp
mov r14, 1
mov rdi, r14
mov r12, r11
mov rsi, r12
mov r10, rsp
mov rdx, r10
syscall
xor rax, rax
mov rdx, rax
mov rax, 0x2000001
syscall

; BB26: 
; op=3
```
</details>
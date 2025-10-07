# Cordell Compiler
Cordell Compiler is a compact hobby compiler for Cordell Programming Language with a simple syntax, inspired by C and Rust. It is designed for studying compilation, code optimization, translation, and low-level microcode generation.

# Main idea of this project
Main goal of this project is learning of compilers architecture and porting one to CordellOS project (I want to code apps for OS inside this OS). Also, according to my bias to assembly and C languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison and etc).

# Usefull links and literature
- Aarne Ranta. *Implementing Programming Languages. An Introduction to Compilers and Interpreters*
- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools (Dragon Book)*
- Andrew W. Appel. *Modern Compiler Implementation in C (Tiger Book)*
- Cytron et al. *Efficiently Computing Static Single Assignment Form and the Control Dependence Graph* (1991)
- Daniel Kusswurm. *Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition*

# Summary
This `README` file contains the main information about this compiler and the development approaches I’ve learned while working on it. This repository also includes a `github.io` site with similar content and some interactive sections. For convenience, a `Navigation` block with quick links to the topics in this file is provided below.

## Navigation
- [Introduction](#introduction)
- [EBNF](#ebnf)
- [Code snippet](#sample-code-snippet)
- [Tokenization part](#tokenization-part)
   - [Example of tokenized code](#example-of-tokenized-code)
- [Markup part](#markup-part)
   - [Example of markup result](#example-of-markup-result)
- [AST part](#ast-part)
   - [Example of AST](#example-of-ast)
   - [AST optimization](#ast-optimization)
- [HIR part](#hir-part)
   - [HIR optimization](#hir-optimization)
   - [Example of HIR](#example-of-hir)
- [CFG part](#cfg-part)
   - [Example of CFG](#example-of-cfg)
- [Dominant calculation](#dominant-calculation)
   - [Strict dominance](#strict-dominance)
   - [Dominance frontier](#dominance-frontier)
- [SSA form](#ssa-form)
   - [Phi function](#phi-function)
- [Liveness analyzer part](#liveness-analyzer-part)
   - [USE and DEF](#use-and-def)
   - [IN and OUT](#in-and-out)
   - [Point of deallocation](#point-of-deallocation)
- [LIR (x86_64) part](#lir-x86_64-part)
   - [LIR x86_64 optimization](#lir-x86_64-optimization)
   - [LIR x86_64 example](#lir-x86_64-example)
- [Codegen (nasm) part](#codegen-nasm-part)

## Introduction
For additional experience, I chose to take on an extra challenge — creating a programming language. This language has an `EBNF-defined` syntax, its own [VS Code extension](https://github.com/j1sk1ss/CordellCompiler/tree/HIR_LIR_SSA/vscode), and documentation. While explaining each layer of the compiler, I will also provide direct examples written in this language.

## EBNF
![EBNF](docs/media/EBNF.jpg)

## Sample code snippet
The code below demonstrates the main capabilities of the language, excluding features such as while loops, syscalls, strings, and other advanced constructs. This code will be referenced in other parts of this `README`, and is presented here as an initial example before all further explanations.
```cpl
{
    function sum(i32 a, i32 b) => i32 {
        arr c[2, i32] = { a, b };
        return c[0] + c[1];
    }

    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 b = 10;
        i32 c = 10;
        i32 d = 10;
        i32 k = 10;
        i32 f = 10;
        if sum(a, b) > (a * b + c + d + k + f); {
            exit 1;
        }
        
        i32 l = ref f;
        exit dref l;
    }
}
```

## Tokenization part
The tokenization part is responsible for splitting the input byte sequence into basic tokens. This module ignores all whitespace and separator symbols (such as newlines and tabs). It also classifies each token into one of the basic types: `number`, `string`, `delimiter`, `comma`, or `dot`.
![tokenization](docs/media/tokenization.png)

### Example of tokenized code
Code above will produce next list of tokens.
```
line=1, type=1, data=[{], 
line=1, type=2, data=[function], 
line=1, type=2, data=[sum], 
line=1, type=1, data=[(], 
line=1, type=2, data=[i32], 
line=1, type=2, data=[a], 
line=1, type=7, data=[,], 
line=1, type=2, data=[i32], 
line=1, type=2, data=[b], 
line=1, type=1, data=[)], 
line=1, type=0, data=[=>], 
<...>
line=16, type=1, data=[}], 
line=17, type=2, data=[i32], 
line=17, type=2, data=[l], 
line=17, type=0, data=[=], 
line=17, type=2, data=[ref], 
line=17, type=2, data=[f], 
line=18, type=6, data=[;], 
line=18, type=2, data=[exit], 
line=18, type=2, data=[dref], 
line=18, type=2, data=[l], 
line=19, type=6, data=[;], 
line=20, type=1, data=[}]
```

## Markup part
The markup stage is the second part of tokenization, but it is separated from the tokenizer in this compiler due to a different design approach. It operates only on the list of tokens and includes support for scopes. The main idea is to perform basic semantic markup of variables — for example, if we declare a variable `i32 a`, all occurrences of `a` within the corresponding scope can be marked as having the `i32` type.
![markup](docs/media/markup.png)

### Example of markup result
```
line=1, type=12, data=[{], 
line=1, type=44, data=[function], 
line=1, type=45, data=[sum], 
line=1, type=10, data=[(], 
line=1, type=24, data=[i32], 
line=1, type=79, data=[a], 
line=1, type=7, data=[,], 
line=1, type=24, data=[i32], 
line=1, type=79, data=[b], 
line=1, type=11, data=[)], 
line=1, type=39, data=[=>],
<...>
line=13, type=6, data=[;], 
line=14, type=12, data=[{], 
line=14, type=38, data=[exit], 
glob line=14, type=3, data=[1], 
line=15, type=6, data=[;], 
line=16, type=13, data=[}], 
line=17, type=24, data=[i32], 
line=17, type=79, data=[l], 
line=17, type=62, data=[=], 
line=17, type=79, data=[f], ref 
line=18, type=6, data=[;], 
line=18, type=38, data=[exit], 
line=18, type=79, data=[l], dref 
line=19, type=6, data=[;], 
line=20, type=13, data=[}]
```

## AST part
Next, we need to parse this sequence of marked tokens to construct an `AST` (Abstract Syntax Tree). There are many approaches to achieve this — for example, `LL` parsing, `LR` parsing, or even `hybrid` techniques that combine `LL` and `LR`. A more complete list of parser types can be found [here](https://www.geeksforgeeks.org/compiler-design/types-of-parsers-in-compiler-design/) or in related compiler design books.
![markup](docs/media/ast.png)

### Example of AST
```
[ block ]
   { scope, id=1 }
      [function] (t=44, v_id=0, s_id=0)
         [sum] (t=45, v_id=0, s_id=0)
            [i32] (t=24, v_id=0, s_id=0)
         { scope, id=2 }
            [i32] (t=24, v_id=0, s_id=0)
               [a] (t=79, v_id=0, s_id=2)
            [i32] (t=24, v_id=0, s_id=0)
               [b] (t=79, v_id=1, s_id=2)
            { scope, id=3 }
               [arr] (t=32, v_id=0, s_id=0)
                  [c] (t=87, v_id=2, s_id=3)
                  [2] (t=3, v_id=0, s_id=0, glob)
                  [i32] (t=24, v_id=0, s_id=0)
                  [a] (t=79, v_id=0, s_id=2)
                  [b] (t=79, v_id=1, s_id=2)
               [return] (t=37, v_id=0, s_id=3)
                  [+] (t=53, v_id=0, s_id=0)
                     [c] (t=87, v_id=2, s_id=3)
                        [0] (t=3, v_id=0, s_id=0, glob)
                     [c] (t=87, v_id=2, s_id=3)
                        [1] (t=3, v_id=0, s_id=0, glob)
      [start] (t=36, v_id=1, s_id=0)
         [i64] (t=23, v_id=0, s_id=0)
            [argc] (t=78, v_id=3, s_id=1)
         [u64] (t=27, ptr, v_id=0, s_id=0)
            [argv] (t=82, ptr, v_id=4, s_id=1)
         [ block ]
            { scope, id=4 }
               [i32] (t=24, v_id=0, s_id=0)
                  [a] (t=79, v_id=5, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [b] (t=79, v_id=6, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [c] (t=79, v_id=7, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [d] (t=79, v_id=8, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [k] (t=79, v_id=9, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [f] (t=79, v_id=10, s_id=4)
                  [10] (t=3, v_id=0, s_id=0, glob)
               [if] (t=51, v_id=0, s_id=4)
                  [>] (t=67, v_id=0, s_id=0)
                     [sum] (t=46, v_id=0, s_id=0)
                        [a] (t=79, v_id=5, s_id=4)
                        [b] (t=79, v_id=6, s_id=4)
                     [+] (t=53, v_id=0, s_id=0)
                        [+] (t=53, v_id=0, s_id=0)
                           [+] (t=53, v_id=0, s_id=0)
                              [+] (t=53, v_id=0, s_id=0)
                                 [*] (t=55, v_id=0, s_id=0)
                                    [a] (t=79, v_id=5, s_id=4)
                                    [b] (t=79, v_id=6, s_id=4)
                                 [c] (t=79, v_id=7, s_id=4)
                              [d] (t=79, v_id=8, s_id=4)
                           [k] (t=79, v_id=9, s_id=4)
                        [f] (t=79, v_id=10, s_id=4)
                  { scope, id=5 }
                     [exit] (t=38, v_id=0, s_id=5)
                        [1] (t=3, v_id=0, s_id=0, glob)
               [i32] (t=24, v_id=0, s_id=0)
                  [l] (t=79, v_id=11, s_id=4)
                  [f] (t=79, ref, v_id=10, s_id=4)
               [exit] (t=38, v_id=0, s_id=4)
                  [l] (t=79, dref, v_id=11, s_id=4)
```

### AST optimization
Now we have a correct `AST` representation of the input code. Before proceeding further, we can optionally perform some optimizations at this stage. We will not spend much time here and will only cover a few examples. Note that `AST-level` optimizations are mostly redundant in this project and are included primarily for learning purposes.
- Condition unrolling: If we have an `if` statement with a constant condition, such as `if 1 { ... }`, or similar constructs with `while` or `switch`, we can unroll them by removing the condition and keeping only the scope that will always execute.
- Dead function elimination: This simple technique removes all unused functions.
- Dead scope elimination: If a scope does not affect the environment, it can be removed.

## HIR part
Now we need to convert our `AST` into a simpler representation. A common approach here is to convert the `AST` into `Three-Address Code` (3AC).
![markup](docs/media/HIR.png)

### HIR optimization
- Constant folding
- HIR Peephole optimization

### Example of HIR
```
{
    fn sum(i32 a, i32 b) -> i32
    {
        alloc i32s a0;
        load_arg(i32s a0);
        alloc i32s b1;
        load_arg(i32s b1);
        {
            prm_st(i32s a0);
            prm_st(i32s b1);
            alloc arrs c2, size: n2;
            i32t tmp12 = arrs c2[n0];
            i32t tmp13 = arrs c2[n1];
            i32t tmp14 = i32t tmp12 + i32t tmp13;
            return i32t tmp14;
        }
    }
    
    start {
        alloc i64s argc3;
        load_starg(i64s argc3);
        alloc u64s argv4;
        load_starg(u64s argv4);
        {
            alloc i32s a5;
            i32t tmp15 = n10 as i32;
            i32s a5 = i32t tmp15;
            alloc i32s b6;
            i32t tmp16 = n10 as i32;
            i32s b6 = i32t tmp16;
            alloc i32s c7;
            i32t tmp17 = n10 as i32;
            i32s c7 = i32t tmp17;
            alloc i32s d8;
            i32t tmp18 = n10 as i32;
            i32s d8 = i32t tmp18;
            alloc i32s k9;
            i32t tmp19 = n10 as i32;
            i32s k9 = i32t tmp19;
            alloc i32s f10;
            i32t tmp20 = n10 as i32;
            i32s f10 = i32t tmp20;
            store_arg(i32s a5);
            store_arg(i32s b6);
            i32t tmp21 = call sum(i32 a, i32 b) -> i32, argc c2;
            i32t tmp22 = i32s a5 * i32s b6;
            i32t tmp23 = i32t tmp22 + i32s c7;
            i32t tmp24 = i32t tmp23 + i32s d8;
            i32t tmp25 = i32t tmp24 + i32s k9;
            i32t tmp26 = i32t tmp25 + i32s f10;
            i32t tmp27 = i32t tmp21 > i32t tmp26;
            if i32t tmp27, goto l73;
            {
                exit n1;
            }
            l73:
            alloc i32s l11;
            u64t tmp28 = &(i32s f10);
            i32t tmp29 = u64t tmp28 as i32;
            i32s l11 = i32t tmp29;
            exit i32s l11;
        }
    }
}
```

## CFG part
With `3AC`, we can move on to `CFG` (Control Flow Graph) creation. There are several ways to split `3AC` into basic blocks. One approach is using `leaders`, while another is to create a block for every command. The second approach is straightforward — each `3AC` instruction becomes its own block. The `leaders` approach, described in the *Dragon Book*, defines three rules for identifying the start of a block:

- The first instruction in a function.
- The target of a JMP instruction.
- The instruction immediately following a JMP.

In this compiler, both approaches are implemented, but for the following explanations, we will use the approach of creating a block for every command.

![markup](docs/media/CFG.png)

### Example of CFG
![markup](docs/media/CFG_example.png)

## Dominant calculation
With the `CFG`, we can determine the dominators of each block. In simple terms, a dominator of a block `Y` is a block `X` that appears on every path from the entry block to `Y`. For example, the following figure illustrates how this works:
![dominators](docs/media/dominators.png)

### Strict dominance
Strict dominance tells us which block strictly dominates another. A block `X` strictly dominates block `Y` if `X` dominates `Y` and `X` != `Y`. Why do we need this? The basic dominance relation marks all blocks that dominate a given block, but later analyses often require only the closest one. A block `X` is said to be the immediate dominator of `Y` if `X` strictly dominates `Y`, and there is no other block `Z` such that `Z` strictly dominates `Y` and is itself strictly dominated by `X`.
![sdom](docs/media/strict_dominance.png)

### Dominance frontier
The dominance frontier of a block `X` is the set of blocks where the dominance of `X` ends. More precisely, it represents all the blocks that are partially influenced by `X`: `X` dominates at least one of their predecessors, but does not dominate the block itself. In other words, it marks the boundary where control flow paths from inside `X’s` dominance region meet paths coming from outside.
![fdom](docs/media/dominance_frontier.png)

## SSA form
Static Single Assignment (SSA) form requires renaming all assigned variables so that each assignment creates a new, unique variable. A simple example is shown below:
![ssa](docs/media/ssa_basic.png)

### Phi function
But here we encounter a problem. What should we do in this specific case?
![ssa_problem](docs/media/ssa_problem.png)

Which version of the variable `a` should be used in the declaration of `b`? The answer is simple — `both`. Here’s the twist: in `SSA` form, we can use a `φ (phi)` function, which tells the compiler which variable version to use. An example of a `φ` function is shown below:
![phi_function](docs/media/phi_function.png)
But how do we determine where to place this function? Here, we use previously computed dominance information. We traverse the entire symbol table of variables. For each variable, we collect the set of blocks where it is defined (either declared or assigned). Then, for each block with a definition, we take its dominance frontier blocks and insert a `φ` function there.
![phi_placement](docs/media/phi_placement.png)
Then, during the SSA renaming process, we keep track of each block that passes through a φ-function block, recording the version of the variable and the block number. This completes the SSA renaming phase, producing the following result:
![phi_final](docs/media/phi_final.png)

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

### Example code
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
        alloc i64s argc0;
        kill c0
        load_starg(i64s argc9);
        kill c9
        alloc u64s argv1;
        kill c1
        load_starg(u64s argv10);
        kill c10
        {
            alloc i32s a2;
            kill c2
            i32t tmp5 = n10 as i32;
            i32s a11 = i32t tmp5;
            kill c5
            alloc u64s b3;
            kill c3
            u64t tmp6 = &(i32s a11);
            u64s b12 = u64t tmp6;
            kill c6
            u64t tmp7 = n11 as u64;
            *(u64s b12) = u64t tmp7;
            kill c11
            kill c12
            kill c7
            alloc i32s c4;
            kill c4
            i32t tmp8 = n10 as i32;
            i32s c13 = i32t tmp8;
            kill c8
            exit i32s c13;
            kill c13
        }
    }
}
```

## Register allocation part
Now that we have the `IN`, `OUT`, `DEF`, and `USE` sets, we can construct an interference graph. The idea is straightforward: we create a vertex for each variable in the symbol table, and then, for every `CFG` block, we connect (i.e., add an edge between) each variable from the block’s `DEF` set with every variable from its `OUT` set. This connection represents that these two variables are live at the same time. The resulting structure is the interference graph, where:
- Vertices represent program variables.
- Edges represent liveness conflicts (interference) between variables.
![ig](docs/media/not_colored_ig.png)

### Graph coloring
Now we can determine which variables can share the same register using graph coloring. The solution to this problem is purely mathematical, and there are many possible strategies to color a graph. In short, the goal is to assign a color to every node (variable) in such a way that no two connected nodes share the same color. The output of this algorithm is a colored interference graph, where each color represents a distinct physical register, and all variables with the same color can safely reuse the same register without overlapping lifetimes.
![colored_ig](docs/media/colored_ig.png)

## LIR (x86_64) part
In the same way as during `HIR` generation, we now produce an intermediate representation similar to `3AC` — but using only two addresses. This step is relatively straightforward, as it primarily involves adapting instructions to the target machine’s addressing model. Because the exact implementation depends heavily on the target architecture (register count, instruction set, addressing modes, etc.), we typically don’t spend much time optimizing or generalizing this layer. Its main goal is simply to bridge the high-level `HIR` representation and the target-specific assembly form, ensuring that each instruction can be directly translated to a valid machine instruction.
![lir_gen](docs/media/lir_gen.png)

### LIR x86_64 optimization

### LIR x86_64 example
```
FDCL func [id=0]
iMOV R13D, EDI
iMOV R12D, ESI
iMOV [RBP - 8], R12D
iMOV [RBP - 4], R13D
iMOV RAX, 0
iMUL RAX, 4
REF RBX, [RBP - 8]
iADD RAX, RBX
GDREF RAX, RAX
iMOV R11D, EAX
iMOV RAX, 1
iMUL RAX, 4
REF RBX, [RBP - 8]
iADD RAX, RBX
GDREF RAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, R11D
iADD RAX, RBX
iMOV R11D, EAX
FRET R11D
STRT
iMOV RAX, [RBP + 8]
iMOV R13, RAX
REF RAX, [RBP + 16]
iMOV R13, RAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 8], EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 16], EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 24], EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV R15D, EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV R14D, EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 32], EAX
PUSH R11
PUSH R12
PUSH R13
PUSH R14
PUSH R15
iMOV EDI, [RBP - 16]
iMOV ESI, [RBP - 8]
FCLL func [id=41]
iMOV R13D, EAX
POP R15
POP R14
POP R13
POP R12
POP R11
iMOV EAX, [RBP - 8]
iMOV EBX, [RBP - 16]
iMUL RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, [RBP - 24]
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, R15D
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, R14D
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, [RBP - 32]
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R13D
iMOV EBX, R11D
iCMP RAX, RBX
iMVZX RAX, AL
iMOV R11D, EAX
iCMP R11D, 0
JNE lb: [vid=73]
EXITOP 1
MKLB lb: [vid=73]
REF EAX, [RBP - 32]
iMOV R12, RAX
iMOV RAX, R12
iMOV EAX, RAX
iMOV R12D, EAX
iMOV EAX, R12D
iMOV R11D, EAX
EXITOP R11D
```

## Codegen (nasm) part
### Example of generated code
```
section .data
section .rodata
section .bss
section .text
global _cpl_start
_cpl_sum:
push rbp
mov rbp, rsp
sub rsp, 8
mov r13d, edi
mov r12d, esi
mov [rbp - 8], r12d
mov [rbp - 4], r13d
mov rbx, 0
imul rbx, 4
lea rax, [rbp - 8]
add rax, rbx
mov rax, [rax]
mov r11d, eax
mov rbx, 1
imul rbx, 4
lea rax, [rbp - 8]
add rax, rbx
mov rax, [rax]
mov r11d, eax
mov rax, r11d
mov rbx, r11d
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rsp, rbp
pop rbp
ret
; op=14
global _start
_start:
push rbp
mov rbp, rsp
sub rsp, 32
mov rax, [rbp + 8]
mov r13, rax
lea rax, [rbp + 16]
mov r13, rax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 8], eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 16], eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 24], eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov r15d, eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov r14d, eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 32], eax
push r11
push r12
push r13
push r14
push r15
mov rdi, [rbp - 16]
mov rsi, [rbp - 8]
call _cpl_[rbp - 8]
mov r13d, eax
pop r15
pop r14
pop r13
pop r12
pop r11
mov rax, [rbp - 8]
mov rbx, [rbp - 16]
imul rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, [rbp - 24]
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, r15d
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, r14d
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, [rbp - 32]
add rax, rbx
mov r11d, eax
mov rax, r13d
mov rbx, r11d
cmp rax, rbx
movzx rax, al
mov r11d, eax
cmp r11d, 0
jne lb73
mov rax, 60
mov rdi, 1
syscall
lb73:
lea rax, [rbp - 32]
mov r12, rax
mov rax, r12
mov eax, rax
mov r12d, eax
mov rax, r12d
mov r11d, eax
mov rax, 60
mov rdi, r11d
syscall
; op=4
```
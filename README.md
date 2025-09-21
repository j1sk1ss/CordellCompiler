# Cordell Compiler Reference
## Navigation
- [Summary](#summary)
- [Architecture](#architecture)
    - [Token generation](#token-generation)
    - [Tokens markup](#tokens-markup)
    - [AST generating](#ast)
    - [Semantic check](#semantic-check)
    - [AST optimization](#ast-optimization)
    - [HIR generation](#hir-generation)
    - [HIR optimization](#hir-optimization)
    - [LIR generation](#lir-generation)
    - [LIR optimization](#lir-optimization)
    - [Micro-code generation](#micro-code-generation)
- [Documentation](#documentation)
    - [Structure](#program-structure)
    - [Variables and Types](#variables-and-types)
    - [Operations](#operations)
    - [Loops and Conditions](#loops-and-conditions)
    - [Functions](#functions)
    - [System calls](#inputoutput-via-system-calls)
- [Examples](#examples)
    - [Printing a number](#example-of-printing-a-number)
    - [Fibonacci N-number print](#example-of-fibonacci-n-number-print)
    - [Simple memory manager](#example-of-simple-memory-manager)
- [Links](#links)

# Summary
**Cordell Compiler** is a compact hobby compiler for `Cordell Programming Language` with a simple syntax, inspired by C and Rust. It is designed for studying compilation, code optimization, translation, and low-level microcode generation. </br>
**Main goal** of this project is learning of compilers architecture and porting one to `CordellOS` project (I want to code apps for OS inside this OS). Also, according to my bias to assembly and C languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison and etc).

# Architecture
## Token generation
Before any work, compiler should split all input text into a list of tokens. In nutshell, [tokens](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/prep/token.c), in this Compiler, looks like this structure:

```C
typedef struct {
    char ro;   /* Is read only flag   */
    char glob; /* Is global flag      */
    char ptr;  /* Is pointer flag     */
    char ref;  /* Is reference flag   */
    char dref; /* Is dereference flag */
    char ext;  /* Is extern flag      */
} tkn_var_info_t;

#define TOKEN_MAX_SIZE 128
typedef struct token {
    tkn_var_info_t vinfo;
    token_type_t   t_type;
    char           value[TOKEN_MAX_SIZE];
    struct token*  next;
    int            lnum; /* Line in source file */
} token_t;
```

Rules of token generation are simple. Here is the list of events, when we cut line and create token:
- If we in token and we met an a line break token (new line or space). 
- If we in token and we met an Unknown bracket (`[`, `]`, `(`, `)`, `{`, `}`). 
- If we in token and we met a different character type (`i32` != `i8`, `i8` != `delim`, `delim` != `comma` and etc.).

Now lets take a look on work example, where we translate this text:
```CPL
{
    extern exfunc printf;
    start(i64 argc, ptr u64 argv) {
        printf("God Bless America! Land that I love! Stand beside her and guide her\nThrough the night with the light from above!")
        exit 0;
    }
}
```

into a list of the tokens:
```
line=1, type=1, data=[{], 
line=1, type=2, data=[extern], 
line=1, type=2, data=[exfunc], 
line=1, type=2, data=[printf], 
line=2, type=7, data=[;], 
line=2, type=2, data=[start], 
line=2, type=1, data=[(], 
line=2, type=2, data=[i64], 
line=2, type=2, data=[argc], 
line=2, type=8, data=[,], 
line=2, type=2, data=[ptr], 
line=2, type=2, data=[u64], 
line=2, type=2, data=[argv], 
line=2, type=1, data=[)], 
line=3, type=1, data=[{], 
line=3, type=2, data=[printf], 
line=3, type=1, data=[(], 
glob line=3, type=84, data=[God Bless America! Land that I love! Stand beside her and guide her
Through the night with the light from above!], 
line=4, type=1, data=[)], 
line=4, type=2, data=[exit], 
glob line=4, type=3, data=[0], 
line=5, type=7, data=[;], 
line=6, type=1, data=[}],
```

## Tokens markup
In this section, we must label the tokens with their base types. For example, variables must be labeled as variables, functions as function definitions, function calls as calls, etc. In `i16`, in this module, we complete the token generation with the final type assignment. An example of how it works is below:
```
line=1, type=1, data=[{], 
line=1, type=2, data=[extern], 
line=1, type=2, data=[exfunc], 
line=1, type=2, data=[printf], 
line=2, type=7, data=[;], 
line=2, type=2, data=[start], 
line=2, type=1, data=[(], 
line=2, type=2, data=[i64], 
line=2, type=2, data=[argc], 
line=2, type=8, data=[,], 
line=2, type=2, data=[ptr], 
line=2, type=2, data=[u64], 
line=2, type=2, data=[argv], 
line=2, type=1, data=[)], 
line=3, type=1, data=[{], 
line=3, type=2, data=[printf], 
line=3, type=1, data=[(], 
glob line=3, type=84, data=[God Bless America! Land that I love! Stand beside her and guide her
Through the night with the light from above!], 
line=4, type=1, data=[)], 
line=4, type=2, data=[exit], 
glob line=4, type=3, data=[0], 
line=5, type=7, data=[;], 
line=6, type=1, data=[}],
```

Result:
```
line=1, type=13, data=[{], 
line=1, type=35, data=[extern], 
line=1, type=43, data=[exfunc], 
line=1, type=45, data=[printf], 
line=2, type=7, data=[;], 
line=2, type=36, data=[start], 
line=2, type=11, data=[(], 
line=2, type=23, data=[i64], 
line=2, type=74, data=[argc], 
line=2, type=8, data=[,], 
line=2, type=27, data=[u64], ptr 
line=2, type=78, data=[argv], ptr 
line=2, type=12, data=[)], 
line=3, type=13, data=[{], 
line=3, type=46, data=[printf], 
line=3, type=11, data=[(], 
glob line=3, type=84, data=[God Bless America! Land that I love! Stand beside her and guide her
Through the night with the light from above!], 
line=4, type=12, data=[)], 
line=4, type=38, data=[exit], 
glob line=4, type=3, data=[0], 
line=5, type=7, data=[;], 
line=6, type=14, data=[}],
```

## AST
A number of compilers generate an Abstract Syntax Tree (next `AST`), and this one of them. The LL(x) alghorithm is simple. Before generating the tree, we already tokenize the entire file, and now we only need register a bunch of parsers for each token type (We will speak about several types of tokens): </br>
- `I64_TYPE_TOKEN` - This token indicates, that the following sequence of tokens is an expression that will be placed into the variable of type `i64`. If we print this structure, it will looks like: </br>

        [I64_TYPE_TOKEN]
            [NAME]
            [expression]
                [...]

- `CALL_TOKEN` - This token tells us, that the next few tokens are the function name and the function's input arguments. Token itself is the function name: </br>

        [CALL_TOKEN (name)]
            [ARG1 expression]
            [ARG2 expression]
            [...]

- `WHILE_TOKEN` - This token similar to `IF_TOKEN`, and tells us about the structure of following tokens: </br>

        [WHILE_TOKEN]
            [STMT]
            [SCOPE]
                [BODY]

- `PLUS_TOKEN` - This is binary operator token, that has next structure: </br>

        [PLUS_TOKEN]
            [LEFT expression]
            [RIGHT expression]

Full text of all rules present [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/cpl_parsers/README.md). Instead of wasting space, lets take a look on the visual example with translation of this code below:
```CPL
{
    extern exfunc printf;
    start(i64 argc, ptr u64 argv) {
        printf("God Bless America! Land that I love! Stand beside her and guide her\nThrough the night with the light from above!")
        exit 0;
    }
}
```

into the `AST`:
```
[ block ]
    { scope, id=1 }
        [extern] (t=35, size=0, off=0, s_id=0)
            [printf] (t=45, size=0, off=0, s_id=0, ext)
        [start] (t=36, size=0, off=0, s_id=0)
            [i64] (t=23, size=8, off=8, s_id=0)
                [argc] (t=74, size=8, off=8, s_id=1)
            [u64] (t=27, size=8, ptr, off=16, s_id=0)
                [argv] (t=78, size=8, ptr, off=16, s_id=1)
            [ block ]
                { scope, id=2 }
                    [printf] (t=46, size=0, off=0, s_id=0, ext)
                        [God Bless America! Land that I love! Stand beside her and guide her
Through the night with the light from above!] (t=84, size=0, off=0, s_id=0, glob)
                    [exit] (t=38, size=0, off=0, s_id=0)
                        [0] (t=3, size=0, off=0, s_id=0, glob)
```

# Semantic check
Semantic module takes care under size, operation and commands correctness. In other words, this module check if code `well-typed`. Additional examples of semantic check can be found [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/sem/README.md)

# AST optimization
- [deadfunc](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/deadfunc.c)
- [strdecl](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/strdecl.c)
- [varinline](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/varinline.c)
- [constopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/constopt.c)
- [condunroll](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/condunroll.c)
- [varuseopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/varuseopt.c)
- [offsetopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/ast/opt/offsetopt.c)
- [deadopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/oast/pt/deadopt.c)

Detailed description of every noted algorithms placed [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/opt/README.md).

# HIR generation
`HIR` is the next presentation of program after `AST`. The main idea here is `AST` flattening for better and simpler optimization. This compiler uses 3AC (3-Address code) for `HIR`. Sample `HIR` of brfk interpreter:
```
========== HIR ==========
MKSCOPE 
   FDCL func: [fid=0]
   FARGLD i64s: [vid=0]
   MKSCOPE 
      VARDECL i64s: [vid=1], 0
      GDREF i64t: [tid=4], i64s: [vid=0]
      MKLB lb: [id=6]
      IFOP i64t: [tid=4], lb: [id=7]
      MKSCOPE 
         iADD i64s: [vid=0], i64s: [vid=0], 1
         iADD i64s: [vid=1], i64s: [vid=1], 1
         ENDSCOPE 
      JMP lb: [id=6]
      MKLB lb: [id=7]
      FRET i64s: [vid=1]
      ENDSCOPE 
   FEND 
   FDCL func: [fid=1]
   FARGLD i64s: [vid=0]
   MKSCOPE 
      PRMST 1
      PRMST 1
      PRMST i64s: [vid=2]
      FARGST i64s: [vid=2]
      FCLL i64t: [tid=19], func: [fid=0], 1
      PRMST i64t: [tid=19]
      SYSC i64t: [tid=22], 4
      FRET i64t: [tid=22]
      ENDSCOPE 
   FEND 
   FDCL func: [fid=2]
   FARGLD i64s: [vid=0]
   FARGLD i64s: [vid=0]
   MKSCOPE 
      PRMST 0
      PRMST 0
      PRMST i64s: [vid=3]
      PRMST i64s: [vid=4]
      SYSC i64t: [tid=31], 4
      FRET i64t: [tid=31]
      ENDSCOPE 
   FEND 
   ARRDECL arrg: [tape], 30000
   ARRDECL arrg: [bracketmap], 10000
   ARRDECL arrg: [stack], 10000
   ARRDECL arrg: [code], 10000
   STRT 
   STARGLD i64s: [vid=9]
   STARGLD u64s: [vid=10]
   MKSCOPE 
      FARGST str: [std_id=0]
      FCLL i64t: [tid=44], func: [fid=1], 1
      FARGST arrg: [code]
      FARGST 10000
      FCLL i64t: [tid=49], func: [fid=2], 2
      VARDECL i32s: [vid=11], i64t: [tid=49]
      VARDECL i32s: [vid=12], 0
      VARDECL i32s: [vid=13], 0
      VARDECL i8s: [vid=14], 43
      VARDECL i8s: [vid=15], 45
      VARDECL i8s: [vid=16], 46
      VARDECL i8s: [vid=17], 44
      VARDECL i8s: [vid=18], 60
      VARDECL i8s: [vid=19], 62
      VARDECL i8s: [vid=20], 91
      VARDECL i8s: [vid=21], 93
      iLWR i32t: [tid=75], i32s: [vid=12], i32s: [vid=11]
      MKLB lb: [id=76]
      IFOP i32t: [tid=75], lb: [id=77]
      MKSCOPE 
         GINDEX i8t: [tid=80], arrg: [code], i32s: [vid=12]
         VARDECL i8s: [vid=22], i8t: [tid=80]
         SWITCHOP i8s: [vid=22], 2
         MKCASE i8s: [vid=20]
         MKSCOPE 
            GINDEX i32t: [tid=89], arrg: [stack], i32s: [vid=13]
            LINDEX i32t: [tid=89], i32s: [vid=13], i32s: [vid=12]
            iADD i32s: [vid=13], i32s: [vid=13], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=21]
         MKSCOPE 
            iLRG i64t: [tid=95], i32s: [vid=13], 0
            IFOP i64t: [tid=95], lb: [id=96]
            MKSCOPE 
               iSUB i32s: [vid=13], i32s: [vid=13], 1
               GINDEX i32t: [tid=102], arrg: [stack], i32s: [vid=13]
               VARDECL i32s: [vid=23], i32t: [tid=102]
               GINDEX i32t: [tid=108], arrg: [bracketmap], i32s: [vid=12]
               LINDEX i32t: [tid=108], i32s: [vid=12], i32s: [vid=23]
               GINDEX i32t: [tid=113], arrg: [bracketmap], i32s: [vid=23]
               LINDEX i32t: [tid=113], i32s: [vid=23], i32s: [vid=12]
               ENDSCOPE 
            JMP lb: [id=97]
            MKLB lb: [id=96]
            MKLB lb: [id=97]
            ENDSCOPE 
         MKENDCASE 
         iADD i64t: [tid=116], i32s: [vid=12], 1
         STORE i32s: [vid=12], i64t: [tid=116]
         ENDSCOPE 
      JMP lb: [id=76]
      MKLB lb: [id=77]
      VARDECL i32s: [vid=24], 0
      VARDECL i32s: [vid=25], 0
      iLWR i32t: [tid=124], i32s: [vid=25], i32s: [vid=11]
      MKLB lb: [id=125]
      IFOP i32t: [tid=124], lb: [id=126]
      MKSCOPE 
         GINDEX i8t: [tid=129], arrg: [code], i32s: [vid=25]
         SWITCHOP i8t: [tid=129], 9
         MKCASE i8s: [vid=19]
         MKSCOPE 
            iADD i32s: [vid=24], i32s: [vid=24], 1
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=18]
         MKSCOPE 
            iSUB i32s: [vid=24], i32s: [vid=24], 1
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=14]
         MKSCOPE 
            GINDEX i8t: [tid=144], arrg: [tape], i32s: [vid=24]
            iADD i8t: [tid=144], i8t: [tid=144], 1
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=15]
         MKSCOPE 
            GINDEX i8t: [tid=151], arrg: [tape], i32s: [vid=24]
            iSUB i8t: [tid=151], i8t: [tid=151], 1
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=16]
         MKSCOPE 
            PRMST 1
            PRMST 1
            iADD i64t: [tid=160], arrg: [tape], i32s: [vid=24]
            PRMST i64t: [tid=160]
            PRMST 1
            SYSC i64t: [tid=162], 4
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=17]
         MKSCOPE 
            PRMST 0
            PRMST 0
            GINDEX i8t: [tid=171], arrg: [tape], i32s: [vid=24]
            REF u64t: [tid=172], i8t: [tid=171]
            PRMST u64t: [tid=172]
            PRMST 1
            SYSC i64t: [tid=174], 4
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=20]
         MKSCOPE 
            GINDEX i8t: [tid=181], arrg: [tape], i32s: [vid=24]
            NOT i8t: [tid=182], i8t: [tid=181]
            IFOP i8t: [tid=182], lb: [id=183]
            MKSCOPE 
               GINDEX i32t: [tid=187], arrg: [bracketmap], i32s: [vid=25]
               STORE i32s: [vid=25], i32t: [tid=187]
               ENDSCOPE 
            JMP lb: [id=184]
            MKLB lb: [id=183]
            MKSCOPE 
               iADD i32s: [vid=25], i32s: [vid=25], 1
               ENDSCOPE 
            MKLB lb: [id=184]
            ENDSCOPE 
         MKENDCASE 
         MKCASE i8s: [vid=21]
         MKSCOPE 
            GINDEX i8t: [tid=194], arrg: [tape], i32s: [vid=24]
            IFOP i8t: [tid=194], lb: [id=195]
            MKSCOPE 
               GINDEX i32t: [tid=199], arrg: [bracketmap], i32s: [vid=25]
               STORE i32s: [vid=25], i32t: [tid=199]
               ENDSCOPE 
            JMP lb: [id=196]
            MKLB lb: [id=195]
            MKSCOPE 
               iADD i32s: [vid=25], i32s: [vid=25], 1
               ENDSCOPE 
            MKLB lb: [id=196]
            ENDSCOPE 
         MKENDCASE 
         MKDEFCASE 
         MKSCOPE 
            iADD i32s: [vid=25], i32s: [vid=25], 1
            ENDSCOPE 
         MKENDCASE 
         ENDSCOPE 
      JMP lb: [id=125]
      MKLB lb: [id=126]
      EXITOP 1
      ENDSCOPE 
   ENDSCOPE
```

# HIR optimization

# LIR generation

# LIR optimization

# Micro-code generation
Additional micro-code generation for `NASM GNU x86_64` arch examples noted [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/src/gen/x86_64_nasm_gnu/README.md). Main examples of microcode generation is `switch` generation, `asm` block generation, float point arithmetic generation and VLA in heap.

- `switch`, in comprarision with `if` generates a binary search structure. For example this code below:
```CPL
{
    start(i64 argc, ptr u64 argv) {
        switch argc; {
            case 1;    { exit 1; }
            case 100;  { exit 3; }
            case 1000; { exit 4; }
            default    { exit 2; }
        }

        exit 0;
    }
}
```

will invoke generation of next asmx86_64 code:
```ASM
section .data
section .rodata
section .bss
section .text
global _start
    _start:
    push rbp
    mov rbp, rsp
    sub rsp, 24
    mov rax, [rbp + 8]
    mov qword [rbp - 16], rax
    lea rax, [rbp + 16]
    mov qword [rbp - 24], rax
    jmp __end_cases_0__

    __case_1_0__:
        mov rax, 1
        mov rdi, 1
        mov rax, 60
        syscall
    jmp __end_switch_0__

    __case_100_0__:
        mov rax, 100
        mov rdi, 3
        mov rax, 60
        syscall
    jmp __end_switch_0__

    __case_1000_0__:
        mov rax, 1000
        mov rdi, 4
        mov rax, 60
        syscall
    jmp __end_switch_0__

    __default_0__:
        mov rdi, 2
        mov rax, 60
        syscall
    jmp __end_switch_0__

    __end_cases_0__:
    mov rax, [rbp - 16]
    cmp rax, 100
    jl __case_l_100_0__
    jg __case_r_100_0__
    jmp __case_100_0__
    __case_l_100_0__:
    cmp rax, 1
    jl __case_l_1_0__
    jg __case_r_1_0__
    jmp __case_1_0__
    __case_l_1_0__:
    jmp __default_0__
    __case_r_1_0__:
    jmp __default_0__
    __case_r_100_0__:
    cmp rax, 1000
    jl __case_l_1000_0__
    jg __case_r_1000_0__
    jmp __case_1000_0__
    __case_l_1000_0__:
    jmp __default_0__
    __case_r_1000_0__:
    jmp __default_0__
    
    __end_switch_0__:
    mov rdi, 0
    mov rax, 60
    syscall
```

- `asm` block inline assembly code into produced generation without any optimization. For instance this code:
```CPL
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 0;
        asm(a) {
            "mov rax, &a",
            "xor rax, rax",
            "mov rax, 0x60",
            "syscall"
        }

        exit 0;
    }
}
```

Will inline assembly:
```ASM
section .data
section .rodata
section .bss
section .text
global _start
_start:
    push rbp
    mov rbp, rsp
    sub rsp, 24
    mov rax, [rbp + 8]
    mov qword [rbp - 16], rax
    lea rax, [rbp + 16]
    mov qword [rbp - 16], rax
    mov dword [rbp - 24], 0

    mov rax, [rbp - 24] ; Inlined block
    xor rax, rax
    mov rax, 0x60
    syscall

    mov rdi, 0
    mov rax, 60
    syscall
```

- `float`/`double` arithmetic based on SIMD registers. Main difference here, that we should save `0.01` in `float`/`double` representation, that's why any operation with those values should go through `SIMD` registers. For example: 
```CPL
{
    start(i64 argc, ptr u64 argv) {
        f64 a = 0.001;
        f64 b = 0.001;
        f64 c = a + b;
        exit 0;
    }
}
```

produce next code:

```ASM
section .data
section .rodata
section .bss
section .text
global _start
_start:
    push rbp
    mov rbp, rsp
    sub rsp, 40
    mov rax, [rbp + 8]
    mov qword [rbp - 16], rax
    lea rax, [rbp + 16]
    mov qword [rbp - 16], rax
    mov qword [rbp - 24], 4562254508917369341 ; 0.01 
    mov qword [rbp - 32], 4562254508917369341 ; 0.01
    mov rbx, [rbp - 24]
    movq xmm1, rbx ; load first value to SIMD register
    mov rax, [rbp - 32]
    movq xmm0, rax ; load second value to SIMD register
    addsd xmm0, xmm1
    movq rax, xmm0
    mov [rbp - 40], rax
    mov rdi, 0
    mov rax, 60
    syscall
```

- `VLA` in heap base on `brk` syscall (`12` in x86_64). Next code:
```CPL
{
    extern i32 size;
    start(i64 argc, ptr u64 argv) {
        
        arr a[size, i32];

        {
            arr b[size, i32];
            
            {
                arr c[10, i32];
                i32 c1 = 10;
            }
        }

        {
            arr d[size, i32];
        }

        exit 0;
    }
}
```

Will produce next microcode:
```ASM
extern size
section .data
section .rodata
section .bss
section .text
global _start
_start:
    push rbp
    mov rbp, rsp
    sub rsp, 64
    mov rax, [rbp + 8]
    mov qword [rbp - 8], rax
    lea rax, [rbp + 16]
    mov qword [rbp - 8], rax

    mov eax, [rel size] ; arr a[size, i32] allocation
    push rax
    mov rax, 12
    mov rdi, 0
    syscall
    mov [rbp - 16], rax
    mov rdi, [rbp - 16]
    pop rbx
    add rax, rbx
    mov rdi, rax
    mov rax, 12
    syscall

    mov eax, [rel size] ; arr b[size, i32] allocation
    push rax
    mov rax, 12
    mov rdi, 0
    syscall
    mov [rbp - 24], rax
    mov rdi, [rbp - 24]
    pop rbx
    add rax, rbx
    mov rdi, rax
    mov rax, 12
    syscall

    mov dword [rbp - 32], 10

    mov rdi, [rbp - 24] ; arr b[size, i32] deallocation
    mov rax, 12
    syscall

    mov eax, [rel size] ; arr d[size, i32] allocation
    push rax
    mov rax, 12
    mov rdi, 0
    syscall
    mov [rbp - 24], rax
    mov rdi, [rbp - 24]
    pop rbx
    add rax, rbx
    mov rdi, rax
    mov rax, 12
    syscall

    mov rdi, [rbp - 24] ; arr d[size, i32] deallocation
    mov rax, 12
    syscall

    mov rdi, [rbp - 16] ; arr a[size, i32] deallocation
    mov rax, 12
    syscall

    mov rdi, 0
    mov rax, 60
    syscall
```

# Short .CPL Documentation
This is a short version of the language documentation. If you are interested, you can find extended document [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/docs/README.md).

## Program Structure
Every program begins with the `start` entrypoint and ends with the `exit [return_code];` statement.

```CPL
1  {
2      start() {
3          exit 0;
4      }
5  }
```

Also every program can contain `pre-implemented` code blocks and data segments:

```CPL
1  {
2      function foo(i64 a = 100) => i64 { return a; }
3      glob i32 b = 0;
4  
5      start() {
6          exit foo();
7      }
8  }
```

## Variables and Types
The following types are supported:

- `f64` - Double value
- `f32` - Float value
- `i64` / `u64` — Integer (64-bit).
- `i32` / `u32` — Integer (32-bit).
- `i16` / `u16` — Integer (16-bit).
- `i8` / `u8`   — Integer (8-bit).
- `str` — String (Array of characters).
- `arr` — Array.

### Declaring Variables
```CPL
1  {
2       i8 a1 = 'C';
3       u8 a2 = 0xFF;
4       i16 a3 = 0b1100110011001100;
5       f64 a4 = 0.001;
6
7       ro u32 a5 = 0777;
8       glob i64 a6 = 31232;
9
10      str msg = "Hello world!";
11      ptr u32 msg_ptr = msg;
12      ptr u64 msg_msg_ptr = ref msg_ptr;
13
14      arr array[10, i32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
15      arr array1[10, u64];
16 }
```

## Operations
Basic arithmetic and logical operations are supported:

| Operation | Description    |
|-----------|----------------|
| `+`       | Addition       |
| `-`       | Subtraction    |
| `*`       | Multiplication |
| `/`       | Division       |
| `%`       | Module         |
| `==`      | Equality       |
| `!=`      | Inequality     |
| `not`     | Negation       |
| `+=` `-=` `*=` `/=`    | Assign operations                           |
| `>` `>=` `<` `<=`      | Comparison                                  |
| `&&` `\|\|`            | Logic operations (Lazy Evaluations support) |
| `>>` `<<` `&` `\|` `^` | Bit operations                              |

## Loops and Conditions
### Switch expression
```CPL
1  {
2      switch (a) {
3          case 1; {
4          }
5          case 111; {
6          }
7          case 1111; {
8          }
9          : ... :
10         default {
11         }
12     }
13 }
```

**Note:** Switch statement based on binary search algorithm, thats why, prefer switch in situations with many cases. In other hand, with three or less options, use if for avoiding overhead.

### If Condition
```CPL
1  {
2      if a > b; {
3      
4      }
5      else {
6      
7      }
8  }
```

### While Loop
```CPL
1  {
2      while (x < 10) && (y > 20); {
3          : ... loop body : 
4      }
5      else {
6          : At the end of the loop :
7      }
8  }
```

## Functions
Functions are declared using the `function` keyword.

```CPL
[modifier] function [name]([type1] [name1] [= def_val], [type2] [name2] [= def_val], ...) [=> ret_type] {
    : function body :
    return something;
}
```

### Examples:
- Definition:
```CPL
1  {
2      glob function sumfunc(i32 a, i32 b = 1) => i32 {
3          return a + b;
4      }
5  
6      function foo() {
7          return;
8      }
9  }
```

- Function call:
```CPL
1  {
2      i32 result = sumfunc(5, 10);
3      : Functions without return values can be called directly :
4      printStr(strptr, size);
5  }
```

## Input/Output (via system calls)
- Inbuild `syscall` function:
```CPL
1  {
2      syscall(1, 1, ptr, size);
3      syscall(0, 0, ptr, size);
4  }
```

- `ASM` block:
```CPL
1  {
2      i32 sysnum = 60;
3      ptr u8 msg = "Hello world!";
4      i32 slen = strlen(msg);
5      asm(sysnum, msg, slen) {
6          "mov rax, &sysnum",
7          "mov rdi, 1",
8          "mov rsi, 1",
9          "mov rdx, &msg",
10         "mov r10, &slen"
11     }
12 }
```

- Function wrap:
```CPL
1  {
2      glob function printStr(ptr i8 buffer, i32 size) => i32 {
3          return syscall(4, 1, buffer, size); 
4      }
5  
6      glob function getStr(ptr i8 buffer, i32 size) => i32 {
7          return syscall(3, 0, buffer, size); 
8      }
9  }
```

## ASM Block
Compiler will inline `ASM` block into produced code with minimal changes (Optimisation don't affect on `ASM` blocks). Main feature here, is variable usage via `&` symbol.
```CPL
1  {
2      u8 a = 0;
3      u8 b = 0;
4      asm(a, b) {
5          "mov rax, &a",
6          "syscall",
7          "mov &b, rax"
8      }
9  }
```

## Comments
Comments are written as annotations `:` within functions and code blocks:

```CPL
1      : Comment in one line :
2       
3      :
4      Function description.
5      Params
6          - name Description
7      :
```

# Examples
If you want see more examples, please look into the folder `examples` or `tests`. Also [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/vscode/README.md) is a VSCode extension for this hobby language.

## Example of Printing a Number:
```CPL
1  extern exfunc printf;
2  
3  {
4      function itoa(ptr i8 buffer, i32 dsize, i32 num) => i32 {
5          i32 index = dsize - 1;
6          i32 tmp = 0;
7  
8          i32 isNegative = 0;
9          if num < 0; {
10             isNegative = 1;
11             num *= -1;
12         }
13 
14         while num > 0; {
15             tmp = num % 10;
16             buffer[index] = tmp + 48;
17             index -= 1;
18             num /= 10;
19         }
20 
21         if isNegative; {
22             buffer[index - 1] = '-';
23         }
24 
25         return 1;
26     }
27 
28     start() {
29         arr buff[32, i8];
30         itoa(buff, 10, 1234567890)
31         printf("%s", buff);
32         exit 0;
33     }
34 }
```

## Example of Fibonacci N-number print:
```CPL
1  from "string.cpl" import itoa;
2  from "stdio.cpl" import prints;
3  
4  {
5      start() {
6          i32 a = 0;
7          i32 b = 1;
8          i32 c = 0;
9          i32 count = 0;
10         while count < 20; {
11             c = a + b;
12             a = b;
13             b = c;
14             
15             arr buffer[40, i8];
16             itoa(buffer, 40, c);
17             prints(buffer, 40);
18 
19             count += 1;
20         }
21 
22         exit 0;
23     }
24 }
```

## Example of simple memory manager:
```CPL
1  {
2      glob arr _mm_head[100000, i8] =;
3      glob arr _blocks_info[100000, i32] =;
4      glob i64 _head = 0;
5  
6      glob function memset(ptr i8 buffer, i32 val, i64 size) => i32 {
7          i64 index = 0;
8          while index < size; {
9              buffer[index] = val;
10             index += 1;
11         }
12 
13         return 1;
14     }
15 
16     glob function malloc(i64 size) => ptr i32 {
17         if size > 0; {
18             ptr i32 curr_mem = _mm_head;
19             i32 block_index = 0;
20             while block_index < 100000; {
21                 if not _blocks_info[block_index]; {
22                     _blocks_info[block_index] = 1;
23                     _blocks_info[block_index + 1] = size;
24                     _blocks_info[block_index + 2] = curr_mem;
25                     return curr_mem;
26                 }
27 
28                 curr_mem = curr_mem + _blocks_info[block_index + 1];
29                 block_index += 3;
30             }
31         }
32 
33         return -1;
34     }
35 
36     glob function free(ptr i32 mem) => i32 {
37         i32 block_index = 0;
38         while block_index < 100000; {
39             if _blocks_info[block_index + 2] == mem; {
40                 _blocks_info[block_index] = 0;
41                 return 1;
42             }
43 
44             block_index += 3;
45         }
46 
47         return 1;
48     }
49 }
```

# Used links and literature
- [Compiler architecture](https://cs.lmu.edu/~ray/notes/compilerarchitecture/)
- [GCC architecture](https://en.wikibooks.org/wiki/GNU_C_Compiler_Internals/GNU_C_Compiler_Architecture)
- [AST tips](https://dev.to/balapriya/abstract-syntax-tree-ast-explained-in-plain-english-1h38)
- [Control flow algorithm](https://en.wikipedia.org/wiki/Control_flow)
- [Summary about optimization](https://en.wikipedia.org/wiki/Optimizing_compiler)
- [acwj. A Compiler Writing Journey](https://github.com/DoctorWkt/acwj)
- Implementing Programming Languages. An Introduction to Compilers and Interpreters. Aarne Ranta.
- Compilers Principles, Techniques, and Tools. Second Edition. Aho Lam Sethi Ullman.
- Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition. Daniel Kusswurm.
- Compilers Principles, Techniques, and Tools. Second Edition. Alfred V. Aho, Monica S. Lam, Ravi Sethi, Jeffrey D. Ullman

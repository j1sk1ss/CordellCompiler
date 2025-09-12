# Cordell Compiler Reference
## Navigation
- [Summary](#summary)
- [Architecture](#architecture)
    - [Token generation](#token-generation)
    - [Tokens markup](#tokens-markup)
    - [AST generating](#ast)
    - [Semantic check](#semantic-check)
    - [Optimization](#optimization)
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
Before any work, compiler should split all input text into a list of tokens. In nutshell, [tokens](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/prep/token.c), in this Compiler, looks like this structure:

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
```
This is sorce file for token generator!
I'm im i:m "squirrel is climbing" \\ // !!!
"''" '""' "'"' '"''""''"' 123asd123asd123asd
Over there! Over there! Over there! 
In the beauty of the Lilies Christ was born across the sea
with the glory in his bossom that transfigures you and me
As he died to make man holy, Let us die to make man free!
While God is marching on!
```

into a list of the tokens:
```
glob=0, line=0, ptr=0, ro=0, type=2, data=This
glob=0, line=0, ptr=0, ro=0, type=2, data=is
glob=0, line=0, ptr=0, ro=0, type=2, data=sorce
glob=0, line=0, ptr=0, ro=0, type=2, data=file
glob=0, line=0, ptr=0, ro=0, type=2, data=for
glob=0, line=0, ptr=0, ro=0, type=2, data=token
glob=0, line=0, ptr=0, ro=0, type=2, data=generator
glob=0, line=1, ptr=0, ro=0, type=0, data=!
glob=0, line=1, ptr=0, ro=0, type=2, data=I
glob=0, line=4, ptr=0, ro=0, type=63, data=m im i:m squirrel is climbing \\ // !!!
    123asd123asd123asd
Over there! Over there! Over there! 
In the beauty of the Lilies  ...
glob=0, line=6, ptr=0, ro=0, type=63, data=Christ was born across the sea
with the glory in his bossom that transfigures you and me
As he died to make man holy, Let us die
```

## Tokens markup
In this section, we must label the tokens with their base types. For example, variables must be labeled as variables, functions as function definitions, function calls as calls, etc. In `i16`, in this module, we complete the token generation with the final type assignment. An example of how it works is below:
```
glob=0, line=1, ptr=0, ro=0, type=1, data={
glob=0, line=1, ptr=0, ro=0, type=2, data=from
glob=0, line=1, ptr=0, ro=0, type=61, data=string.cpl
glob=0, line=1, ptr=0, ro=0, type=2, data=import
glob=0, line=1, ptr=0, ro=0, type=2, data=strlen
glob=0, line=2, ptr=0, ro=0, type=7, data=;
glob=0, line=3, ptr=0, ro=0, type=2, data=function
glob=0, line=3, ptr=0, ro=0, type=2, data=putc
glob=0, line=3, ptr=0, ro=0, type=2, data=i8
glob=0, line=3, ptr=0, ro=0, type=2, data=sym
glob=0, line=3, ptr=0, ro=0, type=7, data=;
glob=0, line=4, ptr=0, ro=0, type=1, data={
glob=0, line=4, ptr=0, ro=0, type=2, data=return
glob=1, line=4, ptr=0, ro=0, type=3, data=1
glob=0, line=5, ptr=0, ro=0, type=7, data=;
glob=0, line=6, ptr=0, ro=0, type=1, data=}
...
glob=0, line=9, ptr=0, ro=0, type=1, data=(
glob=1, line=9, ptr=0, ro=0, type=3, data=1
glob=0, line=9, ptr=0, ro=0, type=8, data=,
glob=1, line=9, ptr=0, ro=0, type=3, data=1
glob=0, line=9, ptr=0, ro=0, type=8, data=,
glob=0, line=9, ptr=0, ro=0, type=2, data=string
glob=0, line=9, ptr=0, ro=0, type=8, data=,
glob=0, line=9, ptr=0, ro=0, type=2, data=strSize
glob=0, line=9, ptr=0, ro=0, type=1, data=)
glob=0, line=10, ptr=0, ro=0, type=7, data=;
glob=0, line=11, ptr=0, ro=0, type=1, data=}
```

Result:
```
glob=0, line=1, ptr=0, ro=0, type=13, data={
glob=0, line=1, ptr=0, ro=0, type=25, data=from
glob=0, line=1, ptr=0, ro=0, type=61, data=string.cpl
glob=0, line=1, ptr=0, ro=0, type=24, data=import
glob=0, line=1, ptr=0, ro=0, type=2, data=strlen
glob=0, line=2, ptr=0, ro=0, type=7, data=;
glob=0, line=3, ptr=0, ro=0, type=31, data=function
glob=0, line=3, ptr=0, ro=0, type=2, data=putc
glob=0, line=3, ptr=0, ro=0, type=21, data=i8
glob=0, line=3, ptr=0, ro=0, type=2, data=sym
glob=0, line=3, ptr=0, ro=0, type=7, data=;
glob=0, line=4, ptr=0, ro=0, type=13, data={
glob=0, line=4, ptr=0, ro=0, type=27, data=return
glob=1, line=4, ptr=0, ro=0, type=3, data=1
glob=0, line=5, ptr=0, ro=0, type=7, data=;
glob=0, line=6, ptr=0, ro=0, type=14, data=}
glob=0, line=7, ptr=0, ro=0, type=31, data=function
...
glob=0, line=9, ptr=0, ro=0, type=27, data=return
glob=0, line=9, ptr=0, ro=0, type=29, data=syscall
glob=0, line=9, ptr=0, ro=0, type=11, data=(
glob=1, line=9, ptr=0, ro=0, type=3, data=1
glob=0, line=9, ptr=0, ro=0, type=8, data=,
glob=1, line=9, ptr=0, ro=0, type=3, data=1
glob=0, line=9, ptr=0, ro=0, type=8, data=,
glob=0, line=9, ptr=0, ro=0, type=2, data=string
glob=0, line=9, ptr=0, ro=0, type=8, data=,
glob=0, line=9, ptr=0, ro=0, type=2, data=strSize
glob=0, line=9, ptr=0, ro=0, type=12, data=)
glob=0, line=10, ptr=0, ro=0, type=7, data=;
glob=0, line=11, ptr=0, ro=0, type=14, data=}
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

Full text of all rules present [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/ast/cpl_parsers/README.md). Instead of wasting space, lets take a look on the visual example with translation of this code below:
```CPL
1  {
2      start(i64 argc, ptr i64 argv) {
3          str stack_str = "String value";
4          ptr str str_ptr = stack_str;
5  
6          i64 a = 0;
7          i32 b = 1;
8          i16 c = 2;
9          i8 d  = 'a';
10 
11         a = b;
12         c = b;
13         d = c;
14         
15         d = strptr;
16         strptr = c;
17 
18         arr large_arr[5, i8] = {1,2,256,4,5,6,7,8,9,10};
19         exit 0;
20     }
21 }
```

into the `AST`:
```
[ block ]
    { scope, id=1 }
        { scope, id=2 }
            [str] (t=22, size=16, off=16, s_id=0)
                [stack_str] (t=60, size=16, off=16, s_id=2)
                [String value] (t=62, size=0, off=0, s_id=0)
            [str] (t=22, size=16, ptr, off=40, s_id=0)
                [str_ptr] (t=60, size=8, ptr, off=24, s_id=2)
                [stack_str] (t=60, size=16, off=16, s_id=2)
            [i64] (t=18, size=8, off=48, s_id=0)
                [a] (t=56, size=8, off=48, s_id=2)
                [0] (t=3, size=0, off=0, s_id=0, glob)
            [i32] (t=19, size=4, off=56, s_id=0)
                [b] (t=57, size=4, off=56, s_id=2)
                [1] (t=3, size=0, off=0, s_id=0, glob)
            [i16] (t=20, size=2, off=64, s_id=0)
                [c] (t=58, size=2, off=64, s_id=2)
                [2] (t=3, size=0, off=0, s_id=0, glob)
            [i8] (t=21, size=1, off=72, s_id=0)
                [d] (t=59, size=1, off=72, s_id=2)
                [a] (t=63, size=8, off=48, s_id=2)
            [=] (t=44, size=0, off=0, s_id=0)
                [a] (t=56, size=8, off=48, s_id=2)
                [b] (t=57, size=4, off=56, s_id=2)
            [=] (t=44, size=0, off=0, s_id=0)
                [c] (t=58, size=2, off=64, s_id=2)
                [b] (t=57, size=4, off=56, s_id=2)
            [=] (t=44, size=0, off=0, s_id=0)
                [d] (t=59, size=1, off=72, s_id=2)
                [c] (t=58, size=2, off=64, s_id=2)
            [=] (t=44, size=0, off=0, s_id=0)
                [d] (t=59, size=1, off=72, s_id=2)
                [strptr] (t=2, size=0, off=0, s_id=0)
            [=] (t=44, size=0, off=0, s_id=0)
                [strptr] (t=2, size=0, off=0, s_id=0)
                [c] (t=58, size=2, off=64, s_id=2)
            [arr] (t=23, size=16, off=88, s_id=0)
                [large_arr] (t=61, size=16, off=0, s_id=0)
                [5] (t=3, size=0, off=0, s_id=0, glob)
                [i8] (t=21, size=0, off=0, s_id=0)
                [1] (t=3, size=0, off=0, s_id=0, glob)
                [2] (t=3, size=0, off=0, s_id=0, glob)
                [256] (t=3, size=0, off=0, s_id=0, glob)
                [4] (t=3, size=0, off=0, s_id=0, glob)
                [5] (t=3, size=0, off=0, s_id=0, glob)
                [6] (t=3, size=0, off=0, s_id=0, glob)
                [7] (t=3, size=0, off=0, s_id=0, glob)
                [8] (t=3, size=0, off=0, s_id=0, glob)
                [9] (t=3, size=0, off=0, s_id=0, glob)
                [10] (t=3, size=0, off=0, s_id=0, glob)
            [exit] (t=28, size=0, off=0, s_id=0)
                [0] (t=3, size=0, off=0, s_id=0, glob)
```

# Semantic check
Semantic module takes care under size, operation and commands correctness. In other words, this module check if code `well-typed`. For instance code above will generate next list of warnings:
```
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 6. Different size [32] (b) and [64] (1). Did you expect this?
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 7. Different size [16] (c) and [64] (2). Did you expect this?
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 10. Different size [64] (a) and [32] (b). Did you expect this?
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 11. Different size [16] (c) and [32] (b). Did you expect this?
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 12. Different size [8] (d) and [16] (c). Did you expect this?
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 14. Different size [8] (d) and [64] (strptr). Did you expect this?
[WARN] (src/prep/semantic.c:21) Danger shadow type cast at line 15. Different size [64] (strptr) and [16] (c). Did you expect this?
[WARN] (src/prep/semantic.c:73) Value 256 at line [17] too large for array [large_arr]!
[WARN] (src/prep/semantic.c:83) Array [large_arr] larger than expected size 10 > 5!
```

Additional examples of semantic check can be found [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/sem/README.md)

# Optimization
- [deadfunc](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadfunc.c)
- [strdecl](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/strdecl.c)
- [varinline](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/varinline.c)
- [constopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/constopt.c)
- [condunroll](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/condunroll.c)
- [varuseopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/varuseopt.c)
- [offsetopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/offsetopt.c)
- [deadopt](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/deadopt.c)

Detailed description of every noted algorithms placed [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/opt/README.md)

# Micro-code generation
Additional micro-code generation for `NASM GNU x86_64` arch examples noted [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/src/gen/x86_64_nasm_gnu/README.md)

# .CPL Documentation
## Program Structure
Every program begins with the `start` entrypoint and ends with the `exit [return_code];` statement.

```CPL
1  {
2      start() {
3          : ... :
4          exit 0;
5      }
6  }
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
3       u8 a2 = 253;
4       i16 a3 = 25093;
5
6       ro u32 a4 = 123123;
7       glob i64 a5 = 31232;
8
9       str msg = "Hello world!";
10      ptr u32 msg_ptr = msg;
11      ptr u64 msg_msg_ptr = ref msg_ptr;
12
13      arr array[10, i32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
14      arr array1[10, u64] =;
15 }
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

### Function Signature:
```CPL
[modifier] function [name]([type1] [name1] [= def_val], [type2] [name2] [= def_val], ...) => [ret_type] {
    : function body :
    return something;
}
```

### Example:
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

### Calling the function
```CPL
1  {
2      i32 result = sumfunc(5, 10);
3      : Functions without return values can be called directly :
4      printStr(strptr, size);
5  }
```

## Input/Output (via system calls)
### String Input/Output
```CPL
1  {
2      syscall(4, 1, ptr, size);
3      syscall(3, 0, ptr, size);
4  }
```

### Wrapping in a function:
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

### ASM Block
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
If you want see more examples, please look into the folder `examples`. Also [here](https://github.com/j1sk1ss/CordellCompiler.PETPRJ/blob/x86_64/vscode/README.md) is a VSCode extension for this hobby language.

### Example of Printing a Number:
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
11             num = num * -1;
12         }
13 
14         while num > 0; {
15             tmp = num % 10;
16             buffer[index] = tmp + 48;
17             index = index - 1;
18             num = num / 10;
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
29         arr buff[32, i8] =;
30         itoa(buff, 10, 1234567890)
31         printf("%s", buff);
32         exit 0;
33     }
34 }
```

### Example of Fibonacci N-number print:
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
15             arr buffer[40, i8] =;
16             itoa(buffer, 40, c);
17             prints(buffer, 40);
18 
19             count = count + 1;
20         }
21 
22         exit 0;
23     }
24 }
```

### Example of simple memory manager:
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
10             index = index + 1;
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
29                 block_index = block_index + 3;
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
44             block_index = block_index + 3;
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

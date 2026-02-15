# CPL changelog
Logs for the first and second versions are quite short because I don’t remember exactly what was introduced and when. However, this page lists most of the major changes. In fact, it was created mainly to document the project’s evolution in a clear way, without a need to read through all of the commits.

## Structure
```
...
^
[Version v3.3]
^
... minor changes related to v3.2 ...
^
[Version v3.2]
^
... minor changes related to v3.1 ...
^
[Version v3.1]
^
... minor changes related to v3 ...
^
[Version v3]
^
[Version v2]
^
[Version v1]
```

----------------------------------------

## Matricies!
Now the compiler supports multi-indexation!
```cpl
extern ptr ptr u32 matrix;
matrix[0][0] = 1;
```

To declare matricies, you will still need to use regular arrays:
```cpl
arr a[2, u32];
arr b[2, u32];
arr c[2, ptr u32] = { ref a, ref b };

c[0][0] = 1; : a[0] :
c[1][0] = 1; : b[0] :
```

## if-elseif-else syntax support
Now this compiler supports the next code snippet:
```cpl
if a; {

}
else if b; {

}
else if c; {

}
: ... :
else {
    
}
```

## De-vertualization (basics)
CorellCompiler now supports polymorphic functions. The symtax is below:
```cpl
{
    function chloe(i32 a, i32 b = 10) => i0;
    function chloe(i64 a, i32 b = 10) => i0;
    start() {
        chloe(10 as i32);
        chloe(10 as i64);
        exit 0;
    }
}
```

The idea that HIR level can choose a function that supports the input arguments. To help the compiler, use the `as` keyword. Additionally, functions with the 'default' argument, for instance:
```cpl
function chloe(i32 a, i32 b = 10) => i0;
```

can't support such polymorphism. The example above works given the same number of the arguemnts. If we consider the next example:
```
function chloe(i32 a, i32 b = 10) => i0;
function chloe(i32 a, i32 b = 10, i32 c = 10) => i0;
```

this mechanism won't work.

## Variadic arguments
Now CPL supports variadic arguments! The syntax is similar to C language:
```cpl
function foo(...) => i0 {
}
```

To pop an argument from this set, use the `poparg` keyword:
```cpl
function max(...) => i0 {
    i32 chloe = poparg as i32;
}
```

Also, the `poparg` keyword can be used in any function with arguments. It simply replaces any argument loading:
```cpl
function foo(i32 a, i32 b) {
    i32 c = poparg; : a :
    i32 d = poparg; : b :
}
```

# Version v3.3
CPL-preprocessing derictives and new include system. </br>
Now this compiler supports derictives such as `define`, `undef`, `ifdef` and `ifndef`. Also it supports the `include` statement. For more information, check the main README file. </br>
For instance let's consider the next piece of code:
```cpl
: string_h.cpl :
{
#ifndef STRING_H_
#define STRING_H_ 0
    : Get the size of the provided string
      Params
        - `s` - Input string.

      Returns the size (i64). :
    function strlen(ptr i8 s) => i64;
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
    function print(ptr str msg) => i0;
#endif
}

: include_test.cpl :
{
    #include "print_h.cpl"
    #include "string_h.cpl"

    function foo() => i0;
    
    function bar() => i0 {
        foo();
    }

    function foo() => i0 {
        print("Hello world!\n");
    }

    start(i64 argc, ptr u64 argv) {
        bar();
        exit 0;
    }
}
```

After the PP, we will get a new form of the code:
```cpl
{
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/print_h.cpl"
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/string_h.cpl"    
    function strlen(ptr i8 s) => i64;
#line 4 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/print_h.cpl"
    function print(ptr str msg) => i0;
#line 2 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/include_test.cpl"
#line 0 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/string_h.cpl"
#line 3 "/Users/nikolaj/Documents/Repositories/CordellCompiler/tests/dummy_data/include_test.cpl"

    function foo() => i0;
    
    function bar() => i0 {
        foo();
    }

    function foo() => i0 {
        print("Hello world!\n");
    }

    start(i64 argc, ptr u64 argv) {
        bar();
        exit 0;
    }
}
```

The `line` derictive here is serving only one purpose - Tell to the tokenizer, where we are.

----------------------------------------

## LiS message
The `lis` statement now accepts a message for a convenient debug.
```cpl
lis;                 : <= Old, but still recognizeble by the compiler :
lis "Debug line #1"; : <= New :
```

These messages will be placed in a final `.asm` file. With the `gdb` the source decompiled file will provide a line where it stops. Produced `.asm` file will include these comments as showed below:
```asm
mov rax, 1
int3 ; Debug line #1
```

## neg, ref and dref now have their own parser
Previously, these staements were affect as flags in tokens. Such a mechanic was unconvenient in cases like below:
```cpl
i32 a = not (c + b);
dref (a + 0x7C00) = 0xDEADBEEF;
```

Now these statements have their own AST and HIR handlers. 

## Explict casting is here!
The CPL language now supports the casting operation. </br>
For instance:
```cpl
i32 a = 10 as i32;
i32 b = 10 as i32;
u8 c = (a + b) as u8;
```

## Additional operators
Implement the next list of operators:
| Operation        | Example   |
|------------------|-----------|
| `%=`             | `X %= Y`  |
| `\|=`            | `X \|= Y` |
| `&=`             | `X &= Y`  |
| `^=`             | `X ^= Y`  |

## Loop statement
Now the `CPL` supports the `loop` statement!
```cpl
loop {
    break;
}
```

This statement the same with the `loop` from Rust.

## Break statement
Now the `CPL` supports the `break` statement!
```cpl
while 1; {
    break;
}

switch 1; {
    case 1; {
        break;
    }
    default {
        break;
    }
}
```

## PTRN DSL
Peephole optimization first phase now is a fully generated by the `PTRN` domain-specific language! Documentation can be seen in the `src/lir/peephole/pattern_generator/README.md`. This allows us to costruct complex and flexible templates for the basic peephole optimization.

## String object
For optimization purpose was implemented a string object. This object responses for the `char*` operations such as `strcat`, `strcmp`, `strcpy`, etc. For better performance the `strcmp` and the `strlen` functions support cache and hash. The `strlen` function simply returns cached value from `string` object, while `strcmp` uses hashes for better string comparison.

# Version v3.2
Now I'm working in the ISP RAS as Research Assistant in the Compiler Department (Static Analyzation team). With some additional experience, now I'm able to implement static analysis in CPL. The basic module contains the semantic entry-point, ast and hir folders. AST folder contains entry point for AST-walker and AST-walker behaviour-scripts. Main idea is simple: We have a walker, that has linked list of actions for different node-types:
```c
typedef enum {
    EXPRESSION_NODE  = 1 << 0,
    ASSIGN_NODE      = 1 << 1,
    DECLARATION_NODE = 1 << 2,
    FUNCTION_NODE    = 1 << 3,
    CALL_NODE        = 1 << 4,
    START_NODE       = 1 << 5,
    DEF_ARRAY_NODE   = 1 << 6,
    UNKNOWN_NODE     = 1 << 7,
} ast_node_type_t;
```

We can register handlers for each node-type. Each handler - different pattern matcher for code check process. Registration is simple:
```c
int SEM_perform_ast_check(ast_ctx_t* actx, sym_table_t* smt) {
    ast_walker_t walker;
    ASTWLK_init_ctx(&walker, smt);

    ASTWLK_register_visitor(ASSIGN_NODE, ASTWLKR_ro_assign, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE | ASSIGN_NODE | EXPRESSION_NODE, ASTWLKR_rtype_assign, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_not_init, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_illegal_declaration, &walker);
    ASTWLK_register_visitor(FUNCTION_NODE, ASTWLKR_no_return, &walker);
    ASTWLK_register_visitor(START_NODE, ASTWLKR_no_exit, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_not_enough_args, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_wrong_arg_type, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_unused_rtype, &walker);
    ASTWLK_register_visitor(DEF_ARRAY_NODE, ASTWLKR_illegal_array_access, &walker);

    ASTWLK_walk(actx, &walker);
    ASTWLK_unload_ctx(&walker);
    return 1;
}
```

The simplest handler is a ASTWLKR_ro_assign handler. Here is the it's source code:
```c
// #define AST_VISITOR_ARGS ast_node_t* nd, sym_table_t* smt
int ASTWLKR_ro_assign(AST_VISITOR_ARGS) {
    ast_node_t* larg = nd->child;
    if (!larg) return 1;
    ast_node_t* rarg = larg->sibling;
    if (!rarg) return 1;

    if (larg->token->flags.ro) {
        SEMANTIC_ERROR(" [line=%i] Read-only variable=%s assign!", larg->token->finfo.line, larg->token->value);
        return 0;
    }

    return 1;
}
```

Also, this version of compiler now operated with ACT (automated commit tool). This tool also is simple, but makes commit section more readeble and "atomic".

### Caller-saving
In the instruction and the memory selection stage here is a one additional step. Now we procceed register saving step for pushing and poping all used in function registers.

### Documentation update
LIR part now is more complex then it was earlier, that's why I start documentation sync process.

### Major refactoring
With custom memmory manager, it's really simple to fix memleaks in entier project. Also, for better code reading in future, I spend some time to make large code refactoring.

### Fixes in inline function
Now inline operation will copy not only variables, now it copies arrays with array elements (if array is local and placed in stack). Also, now SSA renames arrays (local arrays) in same way as it works with variables. Additional function for array's symtable copy function implemented.

### LIR peephole [WIP]
Write optimization (removing unused write operations such as redundant movs). 

### Array args list in HIR and LIR
Instead storing array elements in symtable (in hir_subject form), now it stored in hir and lir array declaration directly. This allows us to perform function inline in more efficient and simple way. In other hand it makes difficult to work with global arrays that are defined in object .data and .rodata sections.

### DFG location
DFG (IN, OUT, DEF and USE calculation) moved from HIR to LIR after instruction selection. Main idea here, preserve registers from rewrite by creating additional interference of tmp variables precolored with used registers in operation.

### Constant propagation
Constant propagation based on DAG and updates data in variable's symtable (works only with constants and numbers). Propagate constants thru:
- Arithmetics
- Convertation
- Copying

### LIR peephole optimization
| Original Instruction        | Optimized Instruction | Explanation |
|-----------------------------|-----------------------|-------------|
| `mov rax, 0`                | `xor rax, rax`        | Zeroes the register without writing to memory; resets flags; usually faster and smaller than `mov`. |
| `sub rax, rax`              | `xor rax, rax`        | Equivalent zeroing; `xor` is generally preferred. |
| `add rax, rax`              | `shl rax, 1`          | Multiply by 2 using shift; can be cheaper than `add` on some CPUs. |
| `imul rax, imm_power_of_2`  | `shl rax, log2(imm)`  | Multiplication by power of 2 replaced with shift. |
| `cmp rax, 0`                | `test rax, rax`       | `test` sets flags like `cmp` but is often cheaper. |
| `mov rax, rax`              | remove                | NOP instruction; useless. |
| `add rax, 0`, `sub rax, 0`  | remove                | Adding zero is a no-op. |
| `imul rax, 1`, `div rax, 1` | remove                | Multiplying by 1 is a no-op. |

### Instruction Planning
Instruction planning will create DAG for each base block, then reorder some instruction depending on target info. Target info - special structure for target CPU arch and machine. For simplicity, I make some python scripts in `src/lir/instplan` directory (`build_targinfo.py` and `read_targinfo.py`).

### Regallocation
Regallocation moved from HIR level to LIR level. Planning to add support of linear and graph register allocation. This move allows me usage of rdx, rdi, rsi... registers, without fear of re-writing (Now I'm able to precolor variables and link them to specific register like ABI registers in function call). 

### Instruction selection
Module for instruction selecting (template section) implemented. In few words: This module lower abstraction to be closer with target machine. Example below.
```lirv1
fn strlen(i8* s) -> i64
{
    %16 = ldparam();
    {
        %18 = %17;
        %19 = %16;
        kill(cnst: 0);
        kill(cnst: 1);
        %17 = num: 0;
        %9 = num: 1 as u64;
        %12 = num: 1 as u64;
        lb10:
        %5 = *(%19);
        %7 = %5 as i64;
        %6 = %7 > num: 10;
        cmp %6, cnst: 0;
        je lb11;
        jmp lb12;
        lb11:
        {
            %8 = %19 / %9;
            %20 = %8;
            %11 = %18 as u64;
            %10 = %11 % %12;
            %13 = %10 as i64;
            %21 = %13;
        }
        %18 = %21;
        %19 = %20;
        jmp lb10;
        lb12:
        return %18;
    }
}
```

Transforms into the code with the support of ABI and specific machine registers (`rax`, `rbx`, `rcx`):
```lirv2
fn strlen(i8* s) -> i64
{
    %16 = rdi;
    {
        %18 = %17;
        %19 = %16;
        kill(cnst: 0);
        kill(cnst: 1);
        %17 = num: 0;
        %9 = num: 1 as u64;
        %12 = num: 1 as u64;
        lb10:
        %5 = *(%19);
        %7 = %5 as i64;
        rax = %7;
        rbx = num: 10;
        cmp rax, rbx;
        setg al;
        %6 zx= al;
        cmp %6, cnst: 0;
        je lb11;
        jmp lb12;
        lb11:
        {
            rax = %19;
            rbx = %9;
            rax = rax / rbx;
            %8 = rax;
            %20 = %8;
            %11 = %18 as u64;
            rax = %11;
            rbx = %12;
            rdx = rdx ^ rdx;
            rdx = rax % rbx;
            %10 = rdx;
            %13 = %10 as i64;
            %21 = %13;
        }
        %18 = %21;
        %19 = %20;
        jmp lb10;
        lb12:
        return %18;
    }
}
```

# Version v3.1
New LIR level. Instead straigthforward LIR generation, now this is a another 3AC level, suitable for instruction selection and instruction planning. Also, instead only register allocation based on graph coloring, this level support register allocation based both on linear scanning approach and graph coloring. 

----------------------------------------

### Inlining 
Function inlined if it reach score higher than 2 points.
```c
static int _inline_candidate(cfg_func_t* f, cfg_block_t* pos) {
    if (!f) return 0;
    int score = 0;

    if (
        pos->type == CFG_LOOP_BLOCK ||
        pos->type == CFG_LOOP_LATCH
    ) score += 2;

    int block_count = list_size(&f->blocks);

    if (block_count <= 2)       score += 3;
    else if (block_count <= 5)  score += 2;
    else if (block_count <= 10) score += 1;
    else if (block_count > 15)  score -= 2;
    
    return score > 2;
}
```

### TRE (tail recursion elimination)
TRE implementation simply do rrcursion elimination if next block after recursion is terminator block (without successors).

### IG fix
Now Interference Graph calculated with `IN`, `DEF` and `OUT` instead only `DEF` and `OUT` sets according to [this](https://courses.cs.cornell.edu/cs4120/2022sp/notes/regalloc/index.html) article.

### AST opt deadfunc
From AST level dead function elimination to HIR level based of call graph.

### SSA LICM optimization
Redundand calculations (instead basic inductions) now moved from loop body to loop preheader.

### CFG BB genration changed
Previous version of BB generation includes complex if operations without two jmps support, that's why leaders from DragonBook works incorrect. Now there is no IFLWR, IFGRT and similar operations, only IFOP2.

### LIR generation based on CFG instead raw HIR
Now LIR generator works only with a CFG data instead of a raw HIR list. Also, the LIR generator produces not only a raw LIR list. Now it produces an updated meta information for base blocks in a CFG (entry and exit for LIR list for asm generator).

### Constant propagation
HIR_DAG_sparse_const_propagation function was implemented. Also there is the new types for numbers and contants (constants and numbers for f/u/i 64/32/16/8). 

### Debug features of CPL
Additional instruction called `lis` (Interesting abbreviation, isn't? This is a LinearIsStop? or is a LiveInputStage? Or... nevermind) and used for setting breakpoints in the code. For example:
```cpl
start() {
    i32 a = 10;
    arr b[123, f64];
    lis; : <- Breakpoint :
    exit 1;
}
```

For usage, run program (executable) with debug tool (like `gdb`, `lldb`).

# Version v3
This is the third version of this compiler. Was performed a full structure transform (from the `token` -> `AST` -> `ASM` structure, that hasn't been changed since the first version, to the `token` -> `AST` -> `HIR` (`CFG` -> `SSA` -> `DAG` -> `CFG`) -> `RA` -> `LIR` -> `ASM` structure). Also, during the development of this version, the `changelog` section was created (on 10.20.2025) though. Additionally, this version is the `optimization-implementation` version. List of the implemented optimizations:
- HIR
    - Constant propagation
    - Constant folding
    - LICM
- LIR
    - MOV optimization

----------------------------------------

# Version v2
This is the second version of this compiler (at the moment of 10.20.2025, at least a stable work version). Main features is a full code refactoring of the `token` part and the `AST` generation. Also perform a cleanup and implement the basic `LIR`. The main improvement was in the syntax of the `CP-language` (Improve the gramma).

```cplv2
extern exfunc printf;
function itoa(ptr i8 buffer, i32 dsize, i32 num) => i32 {
    i32 index = dsize - 1;
    i32 tmp = 0;

    i32 isNegative = 0;
    if num < 0; {
        isNegative = 1;
        num *= -1;
    }

    while num > 0; {
        tmp = num % 10;
        buffer[index] = tmp + 48;
        index -= 1;
        num /= 10;
    }

    if isNegative; {
        buffer[index - 1] = '-';
    }

    return 1;
}

start() {
    arr buff[32, i8];
    itoa(buff, 10, 1234567890)
    printf("%s", buff);
    exit 0;
}
```

Some improvements in typing (now we're able to use Rust-like statements such as `i8`, `u8`, etc.), `asm` blocks, `external` functions, `heap` arrays, etc. This version was also tested via implementation of the `brainfuck interpreter`.

----------------------------------------

# Version v1
This is the first version of this compiler. The last commit before v2 was in the middle of summer of 2025. Main features of this version is a [`token` -> `AST` -> `ASM`] structure, basic `NASM`-syntax code generation, examples like `brainfuck interpreter`, etc. The most interesting part, in my opinion, is the syntax:
```cplv1
function itoa ptr buffer; int dsize; int num; {
    int index = dsize - 1;
    int tmp = 0;

    int isNegative = 0;
    if num < 0; {
        isNegative = 1;
        num = num * -1;
    }

    while num > 0; {
        tmp = num / 10;
        tmp = tmp * 10;
        tmp = num - tmp;
        tmp = tmp + 48;
        buffer[index] = tmp;
        index = index - 1;
        num = num / 10;
    }

    if isNegative == 1; {
        char minus = 45;
        buffer[index - 1] = minus;
    }

    return 1;
}
```

This version wasn't "friendly" as it became now (in the terms of a syntax and a code style). Also, here is the how program's body was looks like:

```cplv1
start
    int a = 0;
    int b = 1;
    int c = 0;
    int count = 0;
    while count < 20; {
        c = a + b;
        a = b;
        b = c;
        
        arr buffer 40 char =;
        itoa buffer 40 c;
        printf buffer 40;

        count = count + 1;
    }
exit 1;
```

Here is a sample from the old README of a function declaration:
```cplv1
function [name] [type1] arg1; [type2] arg2; ...; fstart 
: code ... :
fend [expression]; : return value :
```

Functions were able to handle only the one possible output. Also these functions were have only one possible exit: `fend`. Similar situation with the `if` statement:
```cplv1
if a > b; ifstart
: code ... :
ifend
```

Also that's how I was thinking users should define arrays:
```cplv1
arr sarr 5 int = 1 2 3 4 5;
```

In summary, the first version was very simple. It takes care only about forward tokens translation to an asmcode through `AST` generation.

# CPL changelog
Logs for the first and second versions are quite short because I don’t remember exactly what was introduced and when. However, this page lists most of the major changes. In fact, it was created mainly to document the project’s evolution in a clear way, without the need to read through all the commits.

# Version v3 [WIP]
Third version of this compiler (WIP). Full structure refactoring (from `token` -> `AST` -> `ASM`, that wasn't changed since first version was created, to `token` -> `AST` -> `HIR` (`CFG` -> `SSA` -> `DAG` -> `CFG`) -> `RA` -> `LIR` -> `ASM`). Also this page created during development of this version (10.20.2025). Also this version is optimization implementation version. List of implemented optimizations:
- HIR
    - Constant propagation
    - Constant folding
    - LICM
- LIR
    - MOV optimization

## IG fix
Now Interference Graph calculated with `IN`, `DEF` and `OUT` instead only `DEF` and `OUT` sets according to [this](https://courses.cs.cornell.edu/cs4120/2022sp/notes/regalloc/index.html) article.

## AST opt deadfunc
From AST level dead function elimination to HIR level based of call graph.

## SSA LICM optimization
Redundand calculations (instead basic inductions) now moved from loop body to loop preheader.

## CFG BB genration changed
Previous version of BB generation includes complex if operations without two jmps support, that's why leaders from DragonBook works incorrect. Now there is no IFLWR, IFGRT and similar operations, only IFOP2.

## LIR generation based on CFG instead raw HIR
Now LIR generator works only with CFG data instead raw HIR list. Also, LIR generator produces not only raw LIR list. Now it produces updated meta information for base blocks in CFG (entry and exit for LIR list for asm generator).

## Constant propagation [still WIP]
HIR_DAG_sparse_const_propagation function implemented. Also there is a new types for numbers and contants (constants and numbers for f/u/i 64/32/16/8). 

## Debug features of CPL
Additional instruction called `lis` (Interesting abbreviation, isn't? This is a LinearIsStop? or is a LiveInputStage? Or... nevermind) and used for setting breakpoints in code. Example:
```cpl
start() {
    i32 a = 10;
    arr b[123, f64];
    lis; <- Breakpoint
    exit 1;
}
```

For usage, run program (executable) with debug tool (like `gdb`, `lldb`).

# Version v2 [CURRENT]
Second version of this compiler (currentrly, 10.20.2025, is main work version). Main features is full refactoring of `token` part, `AST` generation cleanup and implementing of basic `LIR`. The main improvement was in syntax of the CP language.

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

Some improvements in typing (`i8`, `u8`, etc.), `asm` blocks, `external` functions, `heap` arrays, and other. This version also was tested with brainfuck interpreter.

# Version v1 [Deprecated]
First version of this compiler. Last commit before v2 was in the middle of summer 2025. Main features of this version is a `token` -> `AST` -> `ASM` structure of the compiler, basic support of a `NASM`, brainfuck interpreter and other stuff. Sample of syntax is here:
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

This version was not too friendly as it now (in syntax and code style). Also here is program body:

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

Also that's how I think users should use arrays:
```cplv1
arr sarr 5 int = 1 2 3 4 5;
```

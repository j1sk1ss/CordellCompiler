# CPL changelog
Logs for the first and second versions are quite short because I don’t remember exactly what was introduced and when. However, this page lists most of the major changes. In fact, it was created mainly to document the project’s evolution in a clear way, without the need to read through all the commits.

----------------------------------------

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
Now LIR generator works only with CFG data instead raw HIR list. Also, LIR generator produces not only raw LIR list. Now it produces updated meta information for base blocks in CFG (entry and exit for LIR list for asm generator).

### Constant propagation
HIR_DAG_sparse_const_propagation function implemented. Also there is a new types for numbers and contants (constants and numbers for f/u/i 64/32/16/8). 

### Debug features of CPL
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

# Version v3
Third version of this compiler (WIP). Full structure refactoring (from `token` -> `AST` -> `ASM`, that wasn't changed since first version was created, to `token` -> `AST` -> `HIR` (`CFG` -> `SSA` -> `DAG` -> `CFG`) -> `RA` -> `LIR` -> `ASM`). Also this page created during development of this version (10.20.2025). Also this version is optimization implementation version. List of implemented optimizations:
- HIR
    - Constant propagation
    - Constant folding
    - LICM
- LIR
    - MOV optimization

----------------------------------------

# Version v2
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

----------------------------------------

# Version v1
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

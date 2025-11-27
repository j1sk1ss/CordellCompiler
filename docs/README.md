# Summary
The **Cordell Programming Language (CPL)** is a system-level programming language designed for learning and experimenting with modern compiler concepts. It combines low-level capabilities from `ASM` with practices inspired by modern languages like `Rust` and `C`. `CPL` is intended for:
- **Systems programming** — operating systems, compilers, interpreters, and embedded software.  
- **Educational purposes** — a language to study compiler design, interpreters, and programming language concepts.  

## Key Features
- **Flexible typing**: variables may hold values of different types; the compiler attempts implicit conversions when assigning.  
- **Explicit memory model**: ownership rules and manual memory management are core features.  
- **Minimalistic syntax**: designed for readability and precision.  
- **Deterministic control flow**: no hidden behaviors; all execution paths are explicit.  
- **Extensibility**: functions and inbuilt macros allow both low-level operations and high-level abstractions.  

# Main idea of this project
Main goal of this project is learning of compilers architecture and porting one to CordellOS project (I want to code apps for OS inside this OS). Also, according to my bias to assembly and C languages (I just love them), this language will stay "low-level" as it possible, but some features can be added in future with strings (inbuild concat, comparison and etc).

# Used links and literature
- Aarne Ranta. *Implementing Programming Languages. An Introduction to Compilers and Interpreters*
- Aho, Lam, Sethi, Ullman. *Compilers: Principles, Techniques, and Tools (Dragon Book)*
- Andrew W. Appel. *Modern Compiler Implementation in C (Tiger Book)*
- K. Vladimirov. *Optimizing Compilers. Structure and Algorithms*
- Cytron et al. *Efficiently Computing Static Single Assignment Form and the Control Dependence Graph* (1991)
- Daniel Kusswurm. *Modern x86 Assembly Language Programming. Covers x86 64-bit, AVX, AVX2 and AVX-512. Third Edition*

# Hello, World! example
```cpl
{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function puts(ptr i8 s) {
        asm (s, strlen(s)) {
            "mov rax, 1",
            "mov rdi, 1",
            "mov rsi, %1",
            "mov rdx, %0",
            "syscall"
        }
        return;
    }

    start(i64 argc, ptr u64 argv) {
        puts("Hello, World!");
        exit 0;
    }
}
``` 

## Code conventions
CPL encourages consistent and readable code.

- **Variables**: use lowercase letters and underscores  
```cpl
i32 counter = 0;
ptr i32 data_ptr = ref counter;
```

- **Constants**: use uppercase letters with underscores
```cpl
extern ptr u8 FRAMEBUFFER;
glob ro i32 WIN_X = 1080;
glob ro i32 WIN_Y = 1920;
```

- **Functions**: use lowercase letters with underscores
```cpl
function calculate_sum(ptr i32 arr, i64 length) => i32 { return 0; }
```

- **Scopes**: K&R style
```cpl
if cond; {
}
while cond; {
}
start(i64 argc, ptr u64 argv) {
}
```

- **Comments**: Comments can be in one line with start and end symbol `:` and in several lines with same logic.
```cpl
: Hello there
:
:
Hello there :
: Hello there :
:
Hello there
:
```

# Program entry point
Function becomes entry point in two ways:
- If this is a `start` function
- If this is the lowest function in file (If there is no `start` function)

Example without defined `start`:
```cpl
function fang() => i0; { return; }
function naomi() => i0; { return; } : <= Becomes entry point :
```

Example with defined `start`:
```cpl
function fang() => i0; { return; }
function naomi() => i0; { return; }
start() { exit 0; } : <= Becomes entry point :
```

# Types
## Primitives
- `f64`, `f32` - double and float; non-floating values are converted to double if used in double operations.
```cpl
f64 a = 0.01;
f32 b = 0.01;
i32 d = 1;
f64 c = a + b + d;
```

- `i64`, `u64` - Long / 64-bit value.
```cpl
i64 a = 123123123;
u64 b = 0b1110011;
i64 c = 0xFFFFFFF;
```

- `i32`, `u32` - Integer / 32-bit value.
```cpl
i32 a = 123123;
u32 b = 0b0111;
i32 c = 0xFFFF;
```

- `i16`, `u16` - Short / 16-bit value.
```cpl
i16 a = 123;
u16 b = 0b1;
i16 c = 0xF;
```

- `i8`, `u8` - Character / 8-bit value.
```cpl
i8 a = 255;
u8 b = 0b0;
i8 c = 0xF;
i8 d = 'a';
```

- `i0` - Void type. Should be used in the function return type.
```cpl
function fang() => i0; { return; }
```

## Strings and arrays
- `str` - String data type. Similar to `ptr u8` type, but used for high-level inbuild operations like `strcmp`.
```cpl
str msg = "Hello world!";
if msg == "Hello world!"; {
}
```

- `arr` - Array data type. Can contain any primitive type.
```cpl
arr arr1[10, i32];
arr arr2[10, i32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
arr matrix[2, u64] = { arr1, arr2 };
```

Also array can have an unkown in `compile-time` size. This will generate code that allocates memory in heap. 
```cpl
extern i8 size;
arr arr1[size, i32];
```

`Runtime-sized` arrays will die when code returns from their home scope. That's why this code below still illegal (Work only in cplv3):
```cpl
extern i8 size;
ptr u8 a;
{
    arr arr1[size, i32];
    a = ref arr1;
}         : <= "arr1" is deallocated. Work with this "a" will cause a SF :
a[0] = 0; : <= SF! :
```

## Pointers
- `ptr` - Pointer modifier that can be add to every primitive (and `str`) type.
```cpl
i32 f = 10;
ptr u64 a = ref f;
ptr str b = "Hello world";
```

# Casting
CPL supports only implicit casting. This means, that any value or return type can be stored in any variable. But semantic module will inform, if it encounter an unexpected implicit casting.
```cpl
{
    function chloe(i8 a) => i32 {
        return 0;
    }

    start(i64 argc, ptr u64 argv) {
        arr a[10, i32];

        a[8] = 1;
        a[11] = 1;

        str msg = "Hello, World!";
        msg[25] = 'A';
    
        chloe();
        i32 a1 = chloe(1, 2, 3);
        chloe(356);
    
        i32 k = 123321;
        i8 f = 123321;
        u8 l = 239;
        i8 l1 = 239;

        ro i8 read;
        read = 1;

        exit 0;
    }
}
```

```bash
[WARNING] [line=1] Variable=a without initialization!
[ERROR]   [line=9] Array=a used with index=11, that larger than array size!
[ERROR]   [line=12] Array=msg used with index=25, that larger than array size!
[ERROR]   [line=15] Too many arguments for function=chloe!
[ERROR]   [line=24] Read-only variable=read assign!
[WARNING] [line=23] Variable=read without initialization!
[WARNING] [line=19] Illegal declaration of f with 123321 (Number bitness is=32, but i8 can handle bitness=8)!
[WARNING] [line=16] Illegal argument of i8 with 356 (Number bitness is=16, but i8 can handle bitness=8)!
[WARNING] [line=16] Unused function=chloe result!
[ERROR]   [line=14] Not enough arguments for function=chloe!
[WARNING] [line=14] Unused function=chloe result!
[WARNING] [line=5] Variable=argv without initialization!
[WARNING] [line=5] Variable=argc without initialization!
```

# Binary and unary operations
| Operation              | Description                                 | Example    |
|------------------------|---------------------------------------------|------------|
| `+`                    | Addition                                    | `X` + `Y`  |
| `-`                    | Subtraction                                 | `X` - `Y`  |
| `*`                    | Multiplication                              | `X` * `Y`  |
| `/`                    | Division                                    | `X` / `Y`  |
| `%`                    | Module                                      | `X` % `Y`  |
| `==`                   | Equality                                    | `X` == `Y` |
| `!=`                   | Inequality                                  | `X` != `Y` |
| `not`                  | Negation                                    | not `X`    |
| `+=` `-=` `*=` `/=`    | Update operations                           | `X` += `Y` |
| `>` `>=` `<` `<=`      | Comparison                                  | `X` >= `Y` |
| `&&` `\|\|`            | Logic operations (Lazy Evaluations support) | `X` && `Y` |
| `>>` `<<` `&` `\|` `^` | Bit operations                              | `X` >> `Y` |

# Scopes
## Variables and lifetime
Variables live in their declared scopes. You cannot point to variables from an outer scope:
```cpl
start() {
   ptr u64 p;
   {
      arr t[10; i32];
      p = ref t; : <= No warning here, but it still illegal :
   }             : <= array "t" died here :

   p[0] = 1;     : <= Pointer to deallocated stack :
   exit 0;
}
```

Note: It will cause memmory corruption error instead `SF` due stack allocation method in CPL.

## Visibility rules
Outer variables can be seen by current and nested scopes.
```cpl
{
   {
      i32 a = 10; : <= Don't see any variables :
   }

   i64 b = 10; : <= Don't see any variables :

   {
      i8 c = 9; : <= See "b" variable :

      {
         f32 a = 10; : <= See "b" and "c" variables :
      }

      i8 a = 0; : <= See "b" and "c" variables :
   }
}
```

# Control flow statements
## if statement
`if` keyword similar to `C` statement. Key change here is `;` after condition. 
```cpl
if cond; {
}
else {
}
```

## while statement
```cpl
while cond; {
}
```

## switch statement
Note: `X` should be constant value (or primitive variable that can be `inlined`).
```cpl
switch cond; {
   case X; {
   }
   default; {
   }
}
```

# Functions and inbuilt macros
## Functions
Functions can be defined by `function` keyword. Also, if you want to use function in another `.cpl`/(or whatever language that support extern) file, you can append `glob` keyword. One note here, that if you want to invoke this function from another language, keep in mind, that CPL change local function name by next pattern: `__cpl_{name}`, that's why prefer mark them with `glob` key. 
```cpl
function min(i32 a) => i0 { return; }
glob function chloe(i32 a = 10) => u64 { return a + 10; }
function max(u64 a = chloe(11)) => i32 { return a + 10; }
```

CPL support default values in functions. Compiler will pass this default args in function call if you don't provide enoght.
```cpl
chloe(); : chloe(10); :
max(); : max(chloe(11)); :
min(max() & chloe()); : min(max(chloe(11)) & chloe(10)); :
```

## Inbuilt macros
There is two inbuild functions that can be usefull for system programmer. First is `syscall` function.
- `syscall` function called similar to default user functions, but can handler variate number of arguments. For example here is the write syscall:
```cpl
str msg = "Hello, World!";
syscall(1, 1, ref msg, strlen(ref msg));
```

- `asm` - Second usefull function that allows inline assembly code. Main feature here is variables line, where you can pass any number of arguments, then use them in assembly code block via `%<num>` symbols.
```cpl
i32 a = 0;
i32 ret = 0;
asm(a, ret) {
   "mov rax, %0 ; mov rax, a",
   "syscall",
   "mov %1, rax ; mov ret, rax"
}
```

Note: Inlined assembly block don't optimized by any alghorithms.

# Ownership rules
## Ownership model vs Rust
CPL uses a lightweight ownership model with `register allocation` that resembles Rust’s borrow checker, but it serves a different purpose and operates with fewer restrictions.  

### Similarities
- **Ownership tracking**:  
  Each variable can have one or more owners. Ownership is explicitly transferred with the `ref` keyword.  
- **Lifetime-based reuse**:  
  When a variable and all of its owners are no longer used, its stack slot can be safely reused by another variable.  
- **Compile-time analysis**:  
  The compiler analyzes usage and ownership before code generation, preventing unsafe reuse of stack slots.

### Differences
1. **Multiple owners allowed**  
   In Rust, only one owner exists at a time, while in CPL multiple owners may coexist in the ownership list.
2. **Optimization-oriented**  
   Rust’s borrow checker enforces memory safety rules.  
   Cordell’s ownership tracking exists primarily to enable **stack slot reuse** and reduce stack frame size.
3. **Memory reuse instead of drops**  
   Rust frees resources automatically at the end of a lifetime (`Drop`).  
   CPL does not guarantee cleanup but instead marks the memory slot as reusable once ownership is gone.
4. **Low-level design**  
   CPL's model is closer to compiler optimizations such as **SSA transformation, register allocation, or stack coloring**, while Rust’s model is a high-level safety feature of the language.

### Example
```cpl
{
    start() {
        i32 a = 0; : <= Allocate 8 bytes :
        ptr i32 p; : <= Allocate 8 bytes :
        if 1; {
            p = ref a; : <= "p" becomes a new owner of "a" :
        }

        i32 c = 0; : <= "p" is still alive, so "a" is not reusable yet :
        exit p;
    }
}
```

# Debugging
## Interrupt point 
Code can be interrupted (use `gdb`/`lldb`) with `lis` keyword. Example below:
```cpl
{
    start() {
        i32 a = 10;
        lis; : <- Will interrupt execution here :
        exit 0;
    }
}
```

Note! Disable optimizations before debugging code due code transforming preservation.

# Examples
## strlen
```cpl
{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }
}
```

## memset
```cpl
{
    function memset(ptr u8 buffer, u8 val, u64 size) => i0 {
        u64 index = 0;
        while index < size; {
            buffer[index] = val;
            index += 1;
        }
    }
}
```

## fd functions
```cpl
{
    function puts(ptr i8 s) => i64 {
        return syscall(1, 1, s, strlen(s));
    }

    function putc(i8 c) => i64 {
        arr tmp[2, i8] = { c, 0 };
        return syscall(1, 1, tmp, 2);
    }

    function gets(ptr i8 buffer, i64 size) => i64 {
        return syscall(0, 0, buffer, size);
    }

    function open(ptr i8 path, i32 flags, i32 mode) => i64 {
        return syscall(2, path, flags, mode);
    }

    function fwrite(i32 fd, ptr u8 buffer, i32 size) => i64 {
        return syscall(1, fd, buffer, size);
    }

    function fread(i32 fd, ptr u8 buffer, i32 size) => i64 {
        return syscall(0, fd, buffer, size);
    }

    function close(i32 fd) => i64 {
        return syscall(3, fd);
    }
}
```

## Brainfuck
```cpl
{
    from "stdio.cpl" import puts, putc, gets;

    glob arr tape[30000, i8];
    glob arr code[10000, i8];
    glob arr bracketmap[10000, i32];
    glob arr stack[10000, i32];

    start() {
        puts("Brainfuck interpriter! Input code: ");

        i32 pos = 0;
        i32 stackptr = 0;
        i32 codelength = gets(code, 10000);
        while pos < codelength; {
            i8 c = code[pos];
            switch c; {
                case '['; {
                    stack[stackptr] = pos;
                    stackptr += 1;
                }
                case ']'; {
                    if stackptr > 0; {
                        stackptr -= 1;
                        i32 matchpos = stack[stackptr];
                        bracketmap[pos] = matchpos;
                        bracketmap[matchpos] = pos;
                    }
                }
            }
            
            pos += 1;
        }
        
        i32 pc = 0;
        i32 pointer = 0;

        while pc < codelength; {
            switch code[pc]; {
                case '>'; {
                    pointer += 1;
                    pc += 1;
                }
                case '<'; {
                    pointer -= 1;
                    pc += 1;
                }
                case '+'; {
                    tape[pointer] += 1;
                    pc += 1;
                }
                case '-'; {
                    tape[pointer] -= 1;
                    pc += 1;
                }
                case '.'; {
                    putc(tape[pointer]);
                    pc += 1;
                }
                case ','; {
                    gets(ref tape[pointer], 1);
                    pc += 1;
                }
                case '['; {
                    if not tape[pointer]; {
                        pc = bracketmap[pc];
                    }
                    else {
                        pc += 1;
                    }
                }
                case ']'; {
                    if tape[pointer]; {
                        pc = bracketmap[pc];
                    }
                    else {
                        pc += 1;
                    }
                }
                default {
                    pc += 1;
                }
            }
        }

        exit 1;
    }
}
```
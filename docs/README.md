# Cordell Programming Language documentation

# Navigation
1. **Introduction**  
   1.1. [Overview](#overview)  
   1.2. [Hello, World! example](#hello-world-example)  
   1.3. [Code conventions](#code-conventions)  
2. **Types**  
   2.1. [Primitives](#primitives)  
   2.2. [Strings and arrays](#strings-and-arrays)  
   2.3. [Pointers](#pointers)  
3. **Casting**  
   - [Type casting rules](#casting)  
4. **Binary operations**  
   - [Operators and precedence](#binary-operations)  
5. **Scopes**  
   5.1. [Variables and lifetime](#variables-and-lifetime)  
   5.2. [Visibility rules](#visibility-rules)  
6. **Control flow statements**  
   6.1. [if statement](#if-statement)  
   6.2. [while statement](#while-statement)  
   6.3. [switch statement](#switch-statement)  
7. **Functions and inbuild macros**  
   7.1. [Functions](#functions)  
   7.2. [Inbuild macros](#inbuild-macros)  
8. **Ownership rules**  
   - [Memory ownership model](#ownership-rules)  
9. **Examples**  
   - [Practical code samples](#examples)  

# Introduction
## Overview
The **Cordell Programming Language (CPL)** is a system-level programming language designed for study and learning how modern compilers work. Also, it combines low-level capabilities from `ASM` with some practices from modern programming languages like `Rust` and `C`.

CPL is intended for:
- **Systems programming** — operating systems, compilers, interpreters, embedded software.
- **Educational use** — as a language to study compilers, interpreters, and language design.

### Key Features
- **Flexible typing**: variables may hold values of different types, and the compiler will attempt implicit conversions when assigning.  
- **Explicit memory model**: ownership rules and manual memory management are part of the language core.  
- **Minimalistic syntax**: designed for readability and precision, avoiding unnecessary keywords.  
- **Deterministic control flow**: no hidden behaviors; all execution paths are explicit.  
- **Extensibility**: functions and built-in macros allow both system-level operations and high-level abstractions.  

## Hello, World! example
```CPL
{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s = s + 1;
            l = l + 1;
        }

        return l;
    }

    function puts(ptr i8 s) {
        i64 l = strlen(s);
        asm (s, l) {
            "mov rax, 1",
            "mov rdi, 1",
            "mov rsi, &s",
            "mov rdx, &l",
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

# Types
## Primitives
## Strings and arrays
## Pointers

# Casting

# Binary operations

# Scopes
## Variables and lifetime
## Visibility rules

# Control flow statements
## if statement
## while statement
## switch statement

# Functions and inbuild macros
## Functions
## Inbuild macros

# Ownership rules
## Ownership model vs Rust
CPL introduces a lightweight ownership model that resembles Rust’s borrow checker, but it serves a different purpose and operates with fewer restrictions.  

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
   Cordell does not guarantee cleanup but instead marks the memory slot as reusable once ownership is gone.
4. **Low-level design**  
   CPL's model is closer to compiler optimizations such as **SSA transformation, register allocation, or stack coloring**, while Rust’s model is a high-level safety feature of the language.

### Example
```CPL
{
    start() {
        : 16 : i32 a = 0;        <= Allocate 8 bytes
        : 24 : ptr i32 p;        <= Allocate 8 bytes
        if 1; {
            p = ref a;           <= "p" becomes new owner of "a"
        }

        : 32 : i32 c = 0;        <= "p" is still alive, so "a" is not reusable yet
        exit p;
    }
}
```

# Examples

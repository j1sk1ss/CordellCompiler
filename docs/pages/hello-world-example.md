# Hello, World! example
That's how we can write a basic 'hello-world' program with CPL language (for x86-64 NASM GNU architecture).
```cpl
: Define the strlen function
  that accepts a pointer to a char array :
function strlen(ptr i8 s) -> i64 {
    i64 l = 0;

    : While pointed symbol isn't a zero value
      continue iteration :
    while dref s; {
        s += 1;
        l += 1;
    }

    : Return the length of the provided
      string :
    return l;
}

: Define the puts function
  that accepts a pointer to a string object :
function puts(str s) -> i0 {
    : Start ASM inline block with
      a support of the argument list :
    asm (s, strlen(s)) {
        "mov rax, 1",
        "mov rdi, 1",
        "mov rsi, %1", : Send the 'strlen(s) result' to the RSI register :
        "mov rdx, %0", : Send the 's' variable to the RDX register       :
        "syscall"
    }
}

: Program entry point similar to the C's entry point
  main(int argc, char* argv[]); :
start(i64 argc, ptr ptr i8 argv) {
    puts("Hello, World!");
    exit 0;
}
``` 

The same code snippet on C language:
```c
#include <stdio.h>
int main(int argc, char* argv[]) {
    puts("Hello, World!");
    return 0;
}
```

Actually, with usage of the same header file, the CPL code can be really close to the C code above:
```cpl
#inclide "stdio_h.cpl"
@[entry("_start")] 
function main(i32 argc, ptr ptr i8 argv) -> i32 {
    puts("Hello, World!");
    exit 0;
}
```

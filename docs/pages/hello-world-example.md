# Hello, World! example
The actual syntax of CPL can be presented with help of a program with a simple functionality to print a welcome message. In a nutshell, that's how we can write a basic 'hello-world' program. </br>
P.S.: *Usage of an assembly block requires to determine the target architecture. In the case below, the target architecture is 'GNU NASM x86_64'.*

```cpl
:/ Define the strlen function
  that accepts a pointer to a char array.
  Params:
    - `s` - Pointer to a string.

  Return a length of the string. /:
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

:/ Define the puts function
  that accepts a pointer to a string object.
  Params:
    - `s` - A string.
    
    Returns 'i0' a/k/a nothing. /:
function puts(str s) -> i0 {
    : Start ASM inline block with
      a support of the argument list :
    asm (s, strlen(s)) {
        "mov rax, 1",
        "mov rdi, 1",
        "mov rsi, %0", : Send the 's' variable to the RSI register       :
        "mov rdx, %1", : Send the 'strlen(s) result' to the RDX register :
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

For comparison here is the same code snippet but on C language:
```c
#include <stdio.h>

int main(int argc, char* argv[]) {
    puts("Hello, World!");
    return 0;
}
```

P.S.: *The C code can looks similar to CPL code if we will abandon the stdlib. But considering that the library is exists and can be used easily, we won't consider an example without it.* </br>
P.S.S.: *Actually, with usage of a similar header file (with the same functions set), the CPL code can looks really close to the C code with the stdlib.*

```cpl
#include "stdio_h.cpl"

@[entry("_start")] function main(i32 argc, ptr ptr i8 argv) -> i32 {
    puts("Hello, World!");
    exit 0;
}
```

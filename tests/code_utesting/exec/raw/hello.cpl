: RUN_ASM :
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
function puts(ptr i8 s) -> i0 {
    : Start ASM inline block with
      a support of the argument list :
    asm (s, strlen(s)) {
        "push rdi", 
        "push rsi", 
        "push rdx",
        "mov rax, 33554436",
        "mov rdi, 1",
        "mov rsi, %0",
        "mov rdx, %1",
        "syscall",
        "pop rdx", 
        "pop rsi", 
        "pop rdi"
    }
}

: Program entry point similar to the C's entry point
  main(int argc, char* argv[]); :
start(i64 argc, ptr ptr i8 argv) {
    puts(ref "Hello, World!\n");
    exit 0;
}

:/ OUTPUT
Hello, World!
/:
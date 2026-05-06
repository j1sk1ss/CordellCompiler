function print(ptr i8 a, i32 len) -> i0 {
    syscall(0x2000004, 1 as i32 as i32, a, len);
}

start() {
    print(0 as ptr i8, 10);
}

:/ OUTPUT
[WARNING] [{X}2:13] NULL-dereference error (variable 'a' is NULL)!
          [{X}2:13]     Variable 'a' is assigned with NULL here
[WARNING] [{X}2:13] Syscall (write, write bytes from a user buffer to a file descriptor) with number 4 has some wrong typed arguments! It can lead to UB, consider to cast them:
          [{X}2:13]     The variable is defined here!
          [{X}2:13]     3 argument (cbuf, user buffer) should have the 'ptr i0' type, but the 'ptr i8' is provided! Consider to cast it with 'as ptr i0'.
[WARNING] [{X}6:11] Function 'print' has some arguments, which have the wrong type! Consider to use the 'as' operator!
          [{X}6:11]     Value '10' has the 'i8' type! Consider the 'as i32' command!
/:
start() {
    syscall(0x2000004, 1, 1, 12);
    syscall(0x2000004, 1 as i32, 1 as ptr i0, 12);
}

:/ OUTPUT
[WARNING] [{X}2:13] Syscall (write, write bytes from a user buffer to a file descriptor) with number 4 has some wrong typed arguments! It can lead to UB, consider to cast them:
          [{X}2:13]     3 argument (cbuf, user buffer) should have the 'ptr i0' type, but the 'i8' is provided! Consider to cast it with 'as ptr i0'.
          [{X}2:13]     2 argument (fd, file descriptor) should have the 'i32' type, but the 'i8' is provided! Consider to cast it with 'as i32'.
/:
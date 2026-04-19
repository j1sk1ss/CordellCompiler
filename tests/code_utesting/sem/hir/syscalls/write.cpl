start() {
    syscall(0x2000004, 1, 1, 12);
    syscall(0x2000004, 1, 1 as ptr i0, 12 as u64);
}

:/ OUTPUT
[WARNING] [2:41] Syscall with number 4 has some wrong typed arguments! It can lead to UB, consider to cast them:
          [2:41]     Argument 2 should have the 'u64' type, but the 'i8' is provided! Consider to cast it.
          [2:41]     Argument 1 should have the 'ptr i0' type, but the 'i8' is provided! Consider to cast it.
/:
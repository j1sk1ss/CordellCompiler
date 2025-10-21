{
    function print_i32(i32 value) {
        arr buf[20, i8];
        i32 i = 18;
        while value > 0; {
            i32 digit = value % 10;
            buf[i] = digit + '0';
            value /= 10;
            i -= 1;
        }

        i += 1;
        i64 len = 20 - i;
        syscall(0x2000004, 1, ref buf + i, len - 1);
        return;
    }

    start(i64 argc, ptr u64 argv) {
        print_i32(100);
        exit 0;
    }
}
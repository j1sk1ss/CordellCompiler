{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

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

    function gets(ptr i8 buffer, i64 size) => i64 {
        return syscall(0x2000003, 0, buffer, size);
    }

    function puts(ptr i8 s) => i64 {
        return syscall(0x2000004, 1, s, strlen(s));
    }

    function atoi(ptr i8 buffer, i32 bsize) => i32 {
        i32 num = 0;
        i32 index = 0;
        while index < bsize; {
            num *= 10;
            i8 sym = buffer[index];
            num += sym - '0';
            index += 1;
        }

        return num;
    }

    start(i64 argc, ptr u64 argv) {
        arr a[16, i8] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        puts("A value: ");
        i64 alen = gets(ref a, 16);

        arr b[16, i8] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        puts("B value: ");
        i64 blen = gets(ref b, 16); 

        i32 aval = atoi(ref a, alen - 1);
        i32 bval = atoi(ref b, blen - 1);

        puts("A + B = ");
        print_i32(aval + bval);
        puts("\n");
        exit aval + bval;
    }
}
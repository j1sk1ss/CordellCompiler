{
    function strlen(ptr i8 s) => i32 {
        i64 l = 0;
        while dref s; {
            s = s + 1;
            l = l + 1;
        }

        return l;
    }

    function puts(ptr i8 s) => i32 {
        i64 size = strlen(s);
        return syscall(1, 1, s, size);
    }

    function gets(ptr i8 buffer, i64 size) => i32 {
        return syscall(0, 0, buffer, size);
    }

    start() {
        glob arr tape[10, i8] =;
        tape[0] = 65; 
        arr buf[2, i8] = { ' ', 0 };
        buf[0] = tape[0];
        puts(buf);
        exit 0;
    }
}

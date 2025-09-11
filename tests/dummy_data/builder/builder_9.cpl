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
        glob arr tape[10, i8] = { 48, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        glob arr code[10, i8] = { 43, 43, 43, 43, 43, 43, 43, 43, 43, 46 };

        i32 pointer = 0;
        i32 pc = 0;
        i32 codelength = 10;

        while pc < codelength; {
            switch code[pc]; {
                case 43; { tape[pointer] = tape[pointer] + 1; pc = pc + 1; }
                case 46; {
                    arr buf[2, i8] =;
                    buf[0] = tape[pointer];
                    puts(buf);
                    pc = pc + 1;
                }
            }
        }

        exit 0;
    }
}

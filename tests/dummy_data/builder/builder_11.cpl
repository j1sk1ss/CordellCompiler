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
        glob arr tape[1, i8] = { 10 };
        glob arr code[4, i8] = { '[', '-', '.', ']' };

        i32 pc = 0;
        i32 pointer = 0;
        i32 codelength = 4;

        while pc < codelength; {
            switch code[pc]; {
                case '-'; { tape[pointer] = tape[pointer] - 1; pc = pc + 1; }
                case '.'; { arr buf[2, i8] = {0,0}; buf[0] = tape[pointer] + 48; puts(buf); pc = pc + 1; }
                case '['; { if not tape[pointer]; { pc = 3; } else { pc = pc + 1; } }
                case ']'; { if tape[pointer]; { pc = 0; } else { pc = pc + 1; } }
            }
        }

        exit 0;
    }

}
{
    start() {
        arr a[64, i8];

        syscall(0x2000004, 1, "Write command: ", 17);
        i32 cmd_len = syscall(0x2000003, 0, ref a, 32);

        i8 pos = 0;
        arr code[100, i8];

        i32 index = 0;
        while index < cmd_len; {
            switch a[index]; {
                case '.'; {
                    syscall(0x2000004, 1, ref code[pos], 1);
                }
                case '+'; { code[pos] += 1; }
                case '-'; { code[pos] -= 1; }
                case '>'; { pos += 1;       }
                case '<'; { pos -= 1;       }
            }

            index += 1;
        }

        exit 0;
    }
}
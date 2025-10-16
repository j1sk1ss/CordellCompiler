{
    start() {
        arr a[32, i8];
        i32 cmd_len = syscall(0x2000003, 0, ref a, 32);

        arr input[2, i8];
        i32 index = 0;
        while index < cmd_len; {
            switch a[index]; {
                case ','; {
                    syscall(0x2000003, 0, ref input, 2);
                }
                case '.'; {
                    syscall(0x2000004, 1, ref input[0], 1);
                }
            }

            index += 1;
        }

        exit 0;
    }
}
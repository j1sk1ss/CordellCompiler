{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function puts(ptr i8 s) => i64 {
        return syscall(0x2000004, 1, s, strlen(s));
    }

    function gets(ptr i8 buffer, i64 size) {
        return syscall(0x2000003, 0, buffer, size);
    }

    start() {
        arr a[32, i8];
        i32 cmd_len = gets(ref a, 32);

        i32 index = 0;
        while index < cmd_len; {
            switch a[index]; {
                case 'a'; {
                    puts("A symbol!\n");
                }
                case 'b'; {
                    puts("B symbol!\n");
                }
                default {
                    puts("Unknown symbol!\n");
                }
            }

            index += 1;
        }

        exit 0;
    }
}
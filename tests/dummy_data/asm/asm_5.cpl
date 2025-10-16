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

    glob arr tape[30000, i8];
    glob arr bracketmap[10000, i32];
    glob arr stack[10000, i32];
    glob arr code[10000, i8];

    start(i64 argc, ptr u64 argv) {
        puts("Brainfuck interpriter! Input code: ");

        i32 codelength = gets(code, 10000);
        i32 pos = 0;
        i32 stackptr = 0;

        while pos < codelength; {
            i8 c = code[pos];
            switch c; {
                case '['; {
                    stack[stackptr] = pos;
                    stackptr += 1;
                }
                case ']'; {
                    if stackptr > 0; {
                        stackptr -= 1;
                        i32 matchpos = stack[stackptr];
                        bracketmap[pos] = matchpos;
                        bracketmap[matchpos] = pos;
                    }
                }
            }
            
            pos += 1;
        }
        
        i32 pointer = 0;
        i32 pc = 0;

        while pc < codelength; {
            switch code[pc]; {
                case '>'; { 
                    pointer += 1;
                    pc += 1;
                }
                case '<'; {
                    pointer -= 1;
                    pc += 1;
                }
                case '+'; {
                    tape[pointer] += 1;
                    pc += 1;
                }
                case '-'; {
                    tape[pointer] -= 1;
                    pc += 1;
                }
                case '.'; {
                    syscall(0x2000004, 1, ref tape[pointer], 1);
                    pc += 1;
                }
                case ','; {
                    syscall(0x2000003, 0, ref tape[pointer], 1);
                    pc += 1;
                }
                case '['; {
                    if not tape[pointer]; {
                        pc = bracketmap[pc];
                    }
                    else {
                        pc += 1;
                    }
                }
                case ']'; {
                    if tape[pointer]; {
                        pc = bracketmap[pc];
                    }
                    else {
                        pc += 1;
                    }
                }
                default {
                    pc += 1;
                }
            }
        }

        exit 1;
    }
}
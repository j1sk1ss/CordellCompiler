{
    function strlen(ptr i8 string) {
        i64 s = 0;
        while dref string; {
            string = string + 1;
            s = s + 1;
        }

        return s;
    }

    function puts(ptr i8 string) {
        i64 size = strlen(string);
        return syscall(1, 1, string, size);
    }

    function gets(ptr i8 buffer, i64 size) {
        return syscall(0, 0, buffer, size);
    }

    glob arr tape[30000, i8];
    glob arr bracketmap[10000, i32];
    glob arr stack[10000, i32];

    start() {
        puts("Brainfuck interpriter! Input code: ");

        i32 codelength = gets(code, 10000);
        arr code[codelength, i8];

        i32 pos = 0;
        i32 stackptr = 0;

        i8 plus          = 43; : + :
        i8 minus         = 45; : - :
        i8 dot           = 46; : . :
        i8 comma         = 44; : , :
        i8 leftMove      = 60; : < :
        i8 rightMove     = 62; : > :
        i8 openBracket   = 91; : [ :
        i8 closedBracket = 93; : ] :

        while pos < codelength; {
            i8 c = code[pos];
            switch c; {
                case openBracket; {
                    stack[stackptr] = pos;
                    stackptr = stackptr + 1;
                }
                case closedBracket; {
                    if stackptr > 0; {
                        stackptr = stackptr - 1;
                        i32 matchpos = stack[stackptr];
                        bracketmap[pos] = matchpos;
                        bracketmap[matchpos] = pos;
                    }
                }
            }
            
            pos = pos + 1;
        }
        
        i32 pointer = 0;
        i32 pc = 0;

        while pc < codelength; {
            switch code[pc]; {
                case rightMove; {
                    pointer = pointer + 1;
                    pc = pc + 1;
                }
                case leftMove; {
                    pointer = pointer - 1;
                    pc = pc + 1;
                }
                case plus; {
                    tape[pointer] = tape[pointer] + 1;
                    pc = pc + 1;
                }
                case minus; {
                    tape[pointer] = tape[pointer] - 1;
                    pc = pc + 1;
                }
                case dot; {
                    syscall(1, 1, tape + pointer, 1);
                    pc = pc + 1;
                }
                case comma; {
                    syscall(0, 0, ref tape[pointer], 1);
                    pc = pc + 1;
                }
                case openBracket; {
                    if not tape[pointer]; {
                        pc = bracketmap[pc];
                    }
                    else {
                        pc = pc + 1;
                    }
                }
                case closedBracket; {
                    if tape[pointer]; {
                        pc = bracketmap[pc];
                    }
                    else {
                        pc = pc + 1;
                    }
                }
                default {
                    pc = pc + 1;
                }
            }
        }

        exit 1;
    }
}
: RUN_ASM[args="-[--->+<]>------.+[--->+<]>++.-------------.++.++++++++.-.[----->++<]>.------------.+[->++<]>+.+[--->+<]>+++..++++.[->+++++<]>+++.[-->+++<]>--.+[--->+<]>+++.-[-->+++++<]>.------------.++++[->++<]>+.+[-->+<]>++.---[->+++<]>+.[->+++++<]>-.+[->+++<]>.++++++++++++.--.----.+++++.-------.-[--->+<]>--.++[->+++<]>.+++++++++.+++.[-->+++++<]>+++.--[->++++<]>+.----------.++++++.-[-->+++++<]>-."] :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

glob arr tape[30000, i8];
glob arr bracketmap[10000, i32];
glob arr stack[10000, i32];

start(i32 argc, ptr ptr i8 argv) {
    i32 pos = 0;
    i32 stackptr = 0;
    i32 codelength = strlen(argv[1]);
    while pos < codelength; {
        @[no_fall]
        @[straight]
        switch argv[1][pos]; {
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
    
    i32 pc = 0;
    i32 pointer = 0;
    while pc < codelength; {
        @[no_fall]
        switch argv[1][pc]; {
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
                putc(tape[pointer]);
                pc += 1;
            }
            case '['; {
                if not tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            case ']'; {
                if tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            default {
                pc += 1;
            }
        }
    }

    exit 0;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
---
@case_index=0
Oregon, CoosBay, I'm coming for you!
---
/:
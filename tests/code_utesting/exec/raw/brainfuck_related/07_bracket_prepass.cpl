: RUN_ASM[args="[[]]"|args="[[.]+]"|args="[]"|args="[[]+]"] :
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

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

@[section(".bss")] glob arr bracketmap[16, i32];
@[section(".bss")] glob arr stack[16, i32];

start(i32 argc, ptr ptr i8 argv) {
    i32 pos = 0;
    i32 stackptr = 0;
    i32 n = strlen(argv[1]);

    while pos < n; {
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
                    i32 m = stack[stackptr];
                    bracketmap[pos] = m;
                    bracketmap[m] = pos;
                }
            }
        }
        pos += 1;
    }

    putdigit(bracketmap[0]);
    putdigit(bracketmap[1]);
    putdigit(bracketmap[2]);
    putdigit(bracketmap[3]);
    putdigit(bracketmap[4]);
    putdigit(bracketmap[5]);
    exit 0;
}

:/ OUTPUT
@case_index=0
321000
---
@case_index=1
530100
---
@case_index=2
100000
---
@case_index=3
421000
---
/:
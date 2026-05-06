: RUN_ASM[args="+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++."] :
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

glob arr tape[64, i8];

start(i32 argc, ptr ptr i8 argv) {
    i32 pc = 0;
    i32 pointer = 0;
    i32 n = strlen(argv[1]);

    while pc < n; {
        @[no_fall]
        switch argv[1][pc]; {
            case '>'; { pointer += 1; }
            case '<'; { pointer -= 1; }
            case '+'; { tape[pointer] += 1; }
            case '-'; { tape[pointer] -= 1; }
            case '.'; { putc(tape[pointer]); }
        }

        pc += 1;
    }

    exit 0;
}

:/ OUTPUT
A
/:
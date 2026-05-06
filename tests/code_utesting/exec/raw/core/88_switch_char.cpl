: RUN_ASM[args=">"|args="<"|args="+"|args="-"|args="."|args="o"] :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start(i32 argc, ptr ptr i8 argv) {
    @[no_fall]
    switch argv[1][0]; {
        case '>'; {
            putc('G');
        }
        case '<'; {
            putc('L');
        }
        case '+'; {
            putc('P');
        }
        case '-'; {
            putc('M');
        }
        case '.'; {
            putc('D');
        }
        default {
            putc('X');
        }
    }

    exit 0;
}

:/ OUTPUT
@case_index=0
G
---
@case_index=1
L
---
@case_index=2
P
---
@case_index=3
M
---
@case_index=4
D
---
@case_index=5
X
---
/:
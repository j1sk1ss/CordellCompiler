: RUN_ASM[args="[][]"|args="[[[[]]]]"|args="[[]"] :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

start(i32 a, ptr ptr i8 argv) {
    i32 pos = 0;
    while pos < strlen(argv[1 as i32]); {
        @[no_fall]
        @[straight]
        switch argv[1 as i32][pos]; {
            case '['; {
                syscall(0x2000004, 1, ref "O", 1);
            }
            case ']'; {
                syscall(0x2000004, 1, ref "C", 1);
            }
        }

        pos += 1;
    }
}

:/ OUTPUT
@case_index=0
OCOC
---
@case_index=1
OOOOCCCC
---
@case_index=2
OOC
---
/:
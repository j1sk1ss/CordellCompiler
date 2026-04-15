: RUN_ASM[args=">"|args="><"|args=">>**"|args=">>*/<<"|args=">*>*>*>*"] :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

start(i32 a, ptr ptr i8 argv) {
    i8 val = 0;
    i32 pos = 0;
    while pos < strlen(argv[1 as i32]); {
        @[no_fall]
        @[straight]
        switch argv[1 as i32][pos]; {
            case '>'; { val += 1; }
            case '<'; { val -= 1; }
            case '*'; { val *= 2; }
            case '/'; { val /= 2; }
        }

        pos += 1;
    }

    exit val;
}

:/ OUTPUT
@case_index=0
@exit_code=1
---
@case_index=1
@exit_code=0
---
@case_index=2
@exit_code=8
---
@case_index=3
@exit_code=0
---
@case_index=4
@exit_code=30
---
/:
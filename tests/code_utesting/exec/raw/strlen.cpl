: RUN_ASM[args="Hello, world!"|args="My name is Max, and I'm from Oregon!"|args="Meh"] :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    exit strlen(argv[1 as i32]) as i8;
}

:/ OUTPUT
@case_index=0
@exit_code=13
---
@case_index=1
@exit_code=36
---
@case_index=2
@exit_code=3
---
/:
: RUN_ASM[args="Hello, World!"|args="asd"|args="CooseBay"] :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    syscall(0x2000004, 1, argv[1 as i32], strlen(argv[1 as i32]));
}

:/ OUTPUT
Hello, World!
asd
CooseBay
/:
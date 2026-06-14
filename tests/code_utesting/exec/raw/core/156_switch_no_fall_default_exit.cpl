: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 x = 5;
    i32 acc = 0;

    @[no_fall]
    switch x; {
        case 1; { acc = 11; }
        case 5; { acc = 22; }
        default { acc = 33; }
    }

    exit acc as u8;
}

:/ OUTPUT
@exit_code=22
/:

: RUN_ASM :

container buffer {
    arr data[5, i8];
}

start() {
    buffer b;

    b.data[0] = 1;
    b.data[1] = 2;
    b.data[2] = 3;
    b.data[3] = 4;
    b.data[4] = 5;

    ptr i8 p = b.data;
    i32 i = 0;
    i32 sum = 0;

    while i < 5; {
        sum += dref p;
        p += 1;
        i += 1;
    }

    exit sum as u8;
}

:/ OUTPUT
@exit_code=15
/:

: RUN_ASM :

container bucket {
    i32 tag;
    arr values[4, i32];
}

function read_i32(ptr i32 p) -> i32 {
    i32 value = dref (p);
    return value;
}

start() {
    bucket b = { 7, 11, 22, 33, 44 };

    ptr i32 base = ref b.values;
    ptr i32 second = ref b.values[1];
    ptr i32 fourth = ref b.values[3];

    if read_i32(base) != 11; exit 1;
    if read_i32(ref b.values) != 11; exit 2;
    if read_i32(second) != 22; exit 3;
    if read_i32(ref b.values[2]) != 33; exit 4;

    dref fourth = 66 as i32;
    exit read_i32(ref b.values[3]) as u8;
}

:/ OUTPUT
@exit_code=66
/:
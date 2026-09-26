: BLOCK_TEST :
: RUN_ASM :

@[section(".bss")]
glob arr values438[3, i32];

@[inline(always)]
function set438(ptr i32 p, i32 index, i32 value) -> i0 {
    p[index] = value;
}

@[inline(never)]
function sum438(ptr i32 p) -> i32 {
    return p[0] + p[1] + p[2];
}

start() {
    set438(ref values438, 0, 7);
    set438(ref values438, 1, 13);
    set438(ref values438, 2, 22);
    exit sum438(ref values438) as u8;
}

:/ OUTPUT
@exit_code=42
/:

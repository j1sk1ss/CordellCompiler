: BLOCK_TEST :
: RUN_ASM :

function inc(i32 x) -> i32 {
    return x + 1;
}

function twice(i32 x) -> i32 {
    return x * 2;
}

container dispatch {
    arr funcs[2, ptr i0];

    @[self]
    function run(ptr dispatch self, i32 index, i32 value) -> i32 {
        return self.funcs[index](value) as i32;
    }
}

start() {
    dispatch d;
    d.funcs[0] = inc;
    d.funcs[1] = twice;

    exit (d.run(0, 20) + d.run(1, 10) + 1) as u8;
}

:/ OUTPUT
@exit_code=42
/:

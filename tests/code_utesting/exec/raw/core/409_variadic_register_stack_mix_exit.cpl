: BLOCK_TEST :
: RUN_ASM :

function sum409(...) -> i64 {
    @[poparg] i8 a;
    @[poparg] i16 b;
    @[poparg] i32 c;
    @[poparg] i64 d;
    @[poparg] u8 e;
    @[poparg] u16 f;
    @[poparg] u32 g;
    @[poparg] u64 h;
    return a as i64 + b as i64 + c as i64 + d + e as i64 + f as i64 + g as i64 + h as i64;
}

start() {
    exit sum409(1 as i8, 2 as i16, 3 as i32, 4 as i64, 5 as u8, 6 as u16, 7 as u32, 14 as u64) as u8;
}

:/ OUTPUT
@exit_code=42
/:

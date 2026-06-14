: BLOCK_TEST :
: RUN_ASM :

container caster_box {
    function twice<T>(T value) -> i32 {
        return (value as i32) * 2;
    }
}

start() {
    caster_box c;

    exit c.twice<i8>(21) as u8;
}

:/ OUTPUT
@exit_code=42
/:

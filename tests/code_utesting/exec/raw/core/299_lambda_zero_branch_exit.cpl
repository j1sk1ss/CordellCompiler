: BLOCK_TEST :
: RUN_ASM :

start() {
    ptr i0 calc = (i64 value) => {
        if value == 0; {
            return 42;
        }

        return value;
    };

    exit calc(0) as u8;
}

:/ OUTPUT
@exit_code=42
/:

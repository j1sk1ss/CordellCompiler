: BLOCK_TEST :
: RUN_ASM :

function choose393(i32 value) -> i32 {
    i32 result = 0;
    switch value; {
        case 1; result += 5;
        case 2; result += 7;
        default result += 30;
    }
    return result;
}

start() {
    exit choose393(1) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

function sum_down(i32 n, i32 acc) -> i32 {
    if n == 0; {
        return acc;
    }

    return sum_down(n - 1, acc + n);
}

start() {
    exit sum_down(6 as i32, 21 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

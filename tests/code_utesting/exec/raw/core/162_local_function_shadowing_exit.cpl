: BLOCK_TEST :
: RUN_ASM :

function outer(i32 x) -> i32 {
    function add(i32 y) -> i32 {
        return y + 4;
    }

    i32 add_result = add(x);
    return add_result + x;
}

start() {
    exit outer(6) as u8;
}

:/ OUTPUT
@exit_code=16
/:

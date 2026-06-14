: BLOCK_TEST :
: RUN_ASM :

@[like_c]
container c_pair {
    i8  tag;
    i32 value;

    @[self]
    function sum(ptr c_pair self) -> i32;
}

function c_pair::sum(ptr c_pair self) -> i32 {
    return self.tag as i32 + self.value;
}

start() {
    c_pair p;
    p.tag = 11;
    p.value = 31;

    exit p.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

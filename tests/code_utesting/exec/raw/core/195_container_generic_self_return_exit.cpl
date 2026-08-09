: BLOCK_TEST :
: RUN_ASM :

container generic_box {
    i32 base;

    @[self]
    function add_as<T>(ptr generic_box self, T value) -> i32 {
        return self.base + value as i32;
    }
}

start() {
    generic_box b;
    b.base = 20;

    exit (b.add_as<i8>(7) + b.add_as<u8>(15)) as u8;
}

:/ OUTPUT
@exit_code=62
/:

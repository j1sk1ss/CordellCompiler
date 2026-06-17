: BLOCK_TEST :
: RUN_ASM :

container overload_counter {
    i32 value;

    @[self]
    function add(ptr overload_counter self, i8 x) -> i0;

    @[self]
    function add(ptr overload_counter self, i32 x) -> i0;
}

function overload_counter::add(ptr overload_counter self, i8 x) -> i0 {
    self.value += x as i32 + 1;
}

function overload_counter::add(ptr overload_counter self, i32 x) -> i0 {
    self.value += x + 2;
}

start() {
    overload_counter c;
    c.value = 10;
    c.add(10 as i8);
    c.add(19 as i32);

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

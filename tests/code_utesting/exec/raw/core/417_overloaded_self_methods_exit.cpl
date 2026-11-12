: BLOCK_TEST :
: RUN_ASM :

container accumulator417 {
    i32 value;
    @[self] function add(ptr accumulator417 self, i8 x) -> i0;
    @[self] function add(ptr accumulator417 self, i32 x) -> i0;
}

function accumulator417::add(ptr accumulator417 self, i8 x) -> i0 {
    self.value += x as i32;
}

function accumulator417::add(ptr accumulator417 self, i32 x) -> i0 {
    self.value += x;
}

start() {
    accumulator417 item;
    item.value = 0;
    item.add(19 as i8);
    item.add(23 as i32);
    exit item.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

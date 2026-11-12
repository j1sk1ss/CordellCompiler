: BLOCK_TEST :
: RUN_ASM :

function plus423(i32 value) -> i32 { return value + 5; }

interface callback423 {
    @[self] @[abstract] function operation(ptr callback423 self) -> ptr i0;
}

container impl423 implements callback423 {
    @[override] function operation(ptr impl423 self) -> ptr i0;
}

function impl423::operation(ptr impl423 self) -> ptr i0 { return plus423; }

start() {
    impl423 item;
    ptr callback423 base = ref item;
    ptr i0 operation = base.operation();
    exit operation(37) as u8;
}

:/ OUTPUT
@exit_code=42
/:

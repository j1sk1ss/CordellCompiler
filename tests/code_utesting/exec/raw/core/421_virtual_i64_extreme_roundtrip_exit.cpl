: BLOCK_TEST :
: RUN_ASM :

interface source421 {
    @[self] @[abstract] function value(ptr source421 self) -> i64;
}

container impl421 implements source421 {
    i64 stored;
    @[override] function value(ptr impl421 self) -> i64;
}

function impl421::value(ptr impl421 self) -> i64 { return self.stored; }

start() {
    impl421 item;
    item.stored = (1 as i64) << 40;
    ptr source421 source = ref item;
    i64 result = source.value() >> 40;
    exit (result + 41) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface value424 { @[self] @[abstract] function sum(ptr value424 self) -> i32; }

@[union]
container impl424 implements value424 {
    u32 word;
    arr bytes[4, u8];
    @[override] function sum(ptr impl424 self) -> i32;
}

function impl424::sum(ptr impl424 self) -> i32 {
    return self.bytes[0] as i32 + self.bytes[1] as i32;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl424 item = place(ref storage, impl424);
    item.word = 0;
    item.bytes[0] = 18;
    item.bytes[1] = 24;
    ptr value424 base = item;
    exit base.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

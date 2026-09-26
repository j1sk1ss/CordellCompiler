: BLOCK_TEST :
: RUN_ASM :

interface value425 { @[self] @[abstract] function get(ptr value425 self) -> i32; }
container first425 implements value425 { @[override] function get(ptr first425 self) -> i32; }
container second425 implements value425 { i32 value; @[override] function get(ptr second425 self) -> i32; }
function first425::get(ptr first425 self) -> i32 { return 1; }
function second425::get(ptr second425 self) -> i32 { return self.value; }

start() {
    @[align(8)] arr storage[64, u8];
    ptr first425 first = place(ref storage, first425);
    ptr value425 old = first;
    i32 before = old.get();
    ptr second425 second = place(ref storage, second425);
    second.value = 41;
    ptr value425 current = second;
    exit (before + current.get()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

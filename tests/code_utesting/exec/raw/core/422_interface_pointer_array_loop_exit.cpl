: BLOCK_TEST :
: RUN_ASM :

interface value422 { @[self] @[abstract] function get(ptr value422 self) -> i32; }
container item422 implements value422 {
    i32 number;
    @[override] function get(ptr item422 self) -> i32;
}
function item422::get(ptr item422 self) -> i32 { return self.number; }

start() {
    item422 a;
    item422 b;
    item422 c;
    a.number = 9;
    b.number = 14;
    c.number = 19;
    arr values[3, ptr value422];
    values[0] = ref a;
    values[1] = ref b;
    values[2] = ref c;
    i32 total = 0;
    i32 i = 0;
    while i < 3; {
        total += values[i].get();
        i += 1;
    }
    exit total as u8;
}

:/ OUTPUT
@exit_code=42
/:

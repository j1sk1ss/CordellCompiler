: BLOCK_TEST :
: RUN_ASM :

#define BASE439 19

function add439(ptr i32 value, i32 delta) -> i0 { dref value += delta; }
function id439<T>(T value) -> T { return value; }

interface value439 { @[self] @[abstract] function get(ptr value439 self) -> i32; }
container impl439 implements value439 {
    i32 number;
    @[override] function get(ptr impl439 self) -> i32;
}
function impl439::get(ptr impl439 self) -> i32 { return self.number; }

function evaluate439(ptr value439 item, ptr i32 bonus) -> i32 {
    defer add439(bonus, 2);
    return item.get();
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl439 item = place(ref storage, impl439);
    item.number = id439<i32>(BASE439 + 21);
    i32 bonus = 0;
    i32 result = evaluate439(item, ref bonus);
    exit (result + bonus) as u8;
}

:/ OUTPUT
@exit_code=42
/:

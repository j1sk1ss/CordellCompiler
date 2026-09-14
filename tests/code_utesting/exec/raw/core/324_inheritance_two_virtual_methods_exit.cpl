: BLOCK_TEST :
: RUN_ASM :

interface iface324 {
    @[self] function left(ptr iface324 self) -> i32;
    @[self] function right(ptr iface324 self) -> i32;
}

container pair324::iface324 {
    @[override] function left(ptr pair324 self) -> i32;
    @[override] function right(ptr pair324 self) -> i32;
}

function pair324::left(ptr pair324 self) -> i32 { 20 }
function pair324::right(ptr pair324 self) -> i32 { 22 }

function call324(ptr iface324 item) -> i32 {
    item.left() + item.right()
}

start() {
    pair324 item;
    exit call324(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

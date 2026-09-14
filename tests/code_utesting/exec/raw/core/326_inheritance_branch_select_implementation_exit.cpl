: BLOCK_TEST :
: RUN_ASM :

interface iface326 {
    @[self] @[abstract]
    function value(ptr iface326 self) -> i32;
}

container low326::iface326 { @[override] function value(ptr low326 self) -> i32; }
container high326::iface326 { @[override] function value(ptr high326 self) -> i32; }
function low326::value(ptr low326 self) -> i32 { return 7; }
function high326::value(ptr high326 self) -> i32 { return 42; }

function choose326(i32 flag, ptr iface326 a, ptr iface326 b) -> i32 {
    if flag; { return b.value(); }
    return a.value();
}

start() {
    low326 a;
    high326 b;
    exit choose326(1 as i32, ref a, ref b) as u8;
}

:/ OUTPUT
@exit_code=42
/:

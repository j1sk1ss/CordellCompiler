#include "iface.inc"
container low26::value26 { @[override] function value(ptr low26 self) -> i32; }
container high26::value26 { @[override] function value(ptr high26 self) -> i32; }
glob low26 LOW26;
glob high26 HIGH26;
function low26::value(ptr low26 self) -> i32 { return 7; }
function high26::value(ptr high26 self) -> i32 { return 42; }
function acquire26(i32 which) -> ptr value26 {
    if which; return ref HIGH26;
    return ref LOW26;
}

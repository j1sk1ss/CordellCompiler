#ifndef CODE_EXT_37_CHILD_INC_
#define CODE_EXT_37_CHILD_INC_ 0
#include "root.inc"
interface child37 implements root37 { @[self] @[abstract] function add(ptr child37 self, i32 delta) -> i0; }
function mutate37(ptr child37 item) -> i0;
#endif

#ifndef CODE_EXT_30_MUL_INC_
#define CODE_EXT_30_MUL_INC_ 0
#include "command.inc"
container mul30 implements command30 { i32 factor; @[override] function execute(ptr mul30 self, ptr context30 context) -> i0; }
#endif

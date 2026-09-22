#ifndef CODE_EXT_30_ADD_INC_
#define CODE_EXT_30_ADD_INC_ 0
#include "command.inc"
container add30 implements command30 { i32 delta; @[override] function execute(ptr add30 self, ptr context30 context) -> i0; }
#endif

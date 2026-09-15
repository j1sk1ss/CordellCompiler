#ifndef CODE_EXT_29_MODEL_INC_
#define CODE_EXT_29_MODEL_INC_ 0
container model29 { i32 value; }
@[abi] extern function ext_bump29(ptr model29 item, i32 delta) -> i0;
function workflow29(ptr model29 item) -> i32;
#endif

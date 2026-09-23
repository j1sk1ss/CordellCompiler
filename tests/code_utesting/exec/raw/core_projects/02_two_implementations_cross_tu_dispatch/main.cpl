#include "add_op.inc"
#include "mul_op.inc"
#include "combine.inc"

start() {
    add_op02 add;
    mul_op02 mul;

    add.bias = 11;
    mul.factor = 2;

    exit combine02(ref add, ref mul, 10 as i32) as u8;
}

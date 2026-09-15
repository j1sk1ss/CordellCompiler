#include "aggregate.inc"

start() {
    add_transform10 add;
    mul_transform10 mul;

    add.bias = 10;
    mul.factor = 3;

    exit aggregate10(ref add, ref mul, 8 as i32) as u8;
}

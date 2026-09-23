#include "add.inc"
#include "mul.inc"
#include "bus.inc"
start() {
    context30 context; add30 add; mul30 mul;
    context.value = 10; add.delta = 4; mul.factor = 3;
    exit run_bus30(ref add, ref mul, ref context) as u8;
}

#include "bus.inc"
function run_bus30(ptr command30 first, ptr command30 second, ptr context30 context) -> i32 {
    arr commands[2, ptr command30];
    commands[0] = first;
    commands[1] = second;
    commands[0].execute(context);
    commands[1].execute(context);
    return context.value;
}

function foo() -> i32;
function bar() -> i0;
start() {
    i32 a = bar();
    foo();
    ptr i32 b = ref (10 + 10);
}

:/ OUTPUT
[{X}4:9] Narrow implicit cast is forbidden!
[WARNING] [{X}4:9] Function 'bar' doesn't return any value, but it used as a value. Consider to change the return type.
[WARNING] [{X}5:9] Function 'foo' has the 'i32' return type but the call doesn't store it anywhere else.
[WARNING] [{X}6:14] The danger reference to a temp value! Consider to reference from a variable with this value.
/:
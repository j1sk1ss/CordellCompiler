function foo() {
    i32 a = @[sizeof]1;
    i32 b = @[sizeof]a;
    i32 c = @[sizeof] (a + b);
    arr d[10, i32];
    i32 e = @[sizeof]d / @[sizeof]d[0];
}

: OUTPUT
@[sizeof] base_token=1
@[sizeof] base_token=a
@[sizeof] base_token=+
@[sizeof] base_token=d
@[sizeof] base_token=<unknown>
:
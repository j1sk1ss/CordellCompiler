{
    function pow(i32 num = 1, i8 power = 1) => i64 {
        return 1;
    }

    glob function foo(i32 a = pow()) {
        return a;
    }

    function void() {
        return;
    }

    start() {
        i32 b = 10;
        foo(1);
        foo();
        void();
        exit pow(b, 2);
    };
}
{
    glob function foo() -> i32;
    start() {
        glob i32 a;
        glob ro i32 b;
        ro glob i32 c;
        ro i32 d;
        ro glob ptr i32 e;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32g %0 = alloc(8);
                i32g %1 = alloc(8);
                i32g %2 = alloc(8);
                i32g %3 = alloc(8);
                i32g* %4 = alloc(8);
            }
        }
    }
}
:
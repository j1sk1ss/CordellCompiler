function foo() -> i0;
function foo(i32 a) -> i0;
start() {
    foo();
    foo(10 as i32);
    exit 0;
}

:/ OUTPUT
{
    start {
        {
            {
                foo0();
                i32t %1 = i8n 10 as i32;
                use i32t %1;
                foo1(i32t %1);
                exit i8n 0;
            }
        }
    }
}
/:
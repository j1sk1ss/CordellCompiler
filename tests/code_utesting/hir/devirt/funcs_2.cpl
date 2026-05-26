function foo(i32 a) -> i0;
function foo(i64 a) -> i0;
start() {
    foo(10 as i64);
    foo(10 as i32);
    exit 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i64t %2 = i8n 10 as i64;
            use i64t %2;
            foo1(i64t %2);
            i32t %3 = i8n 10 as i32;
            use i32t %3;
            foo0(i32t %3);
            u8t %4 = i8n 0 as u8;
            exit u8t %4;
        }
    }
}
/:
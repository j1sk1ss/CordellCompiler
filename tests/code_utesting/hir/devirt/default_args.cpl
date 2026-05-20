function foo(i32 a, i32 b = 20);
function foo(i8 a, i32 b = 20);
start() {
    foo(10 as i32);
    foo(20 as i8);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %4 = i8n 10 as i32;
            use i32t %4;
            foo0(i32t %4, i8n 20);
            i8t %5 = i8n 20 as i8;
            use i8t %5;
            foo1(i8t %5, i8n 20);
        }
    }
}
/:
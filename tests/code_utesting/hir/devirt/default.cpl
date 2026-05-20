function foo(i32 a, i32 b = 10, i32 c = 10);
function foo(i8 a, i32 b = 20, i32 c = 20);
function foo(i16 a, i64 b = 30, i8 c = 40);
start() {
    foo(10 as i32);
    foo(10 as i8);
    foo(10 as i16);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %9 = i8n 10 as i32;
            use i32t %9;
            foo0(i32t %9, i8n 10, i8n 10);
            i8t %10 = i8n 10 as i8;
            use i8t %10;
            foo1(i8t %10, i8n 20, i8n 20);
            i16t %11 = i8n 10 as i16;
            use i16t %11;
            foo2(i16t %11, i8n 30, i8n 40);
        }
    }
}
/:
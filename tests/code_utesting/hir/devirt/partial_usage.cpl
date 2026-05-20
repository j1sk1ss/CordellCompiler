function foo(i32 a, i32 b, i32 c);
function foo(i8 a, i8 b, i8 c);
function foo(i16 a, i16 b, i16 c);
start() {
    foo(10 as i32, 10, 10);
    foo(10 as i16, 10, 10);
    foo(10 as i8, 10, 10);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %9 = i8n 10 as i32;
            use i32t %9;
            foo0(i32t %9, i8n 10, i8n 10);
            i16t %10 = i8n 10 as i16;
            use i16t %10;
            foo2(i16t %10, i8n 10, i8n 10);
            i8t %11 = i8n 10 as i8;
            use i8t %11;
            foo1(i8t %11, i8n 10, i8n 10);
        }
    }
}
/:
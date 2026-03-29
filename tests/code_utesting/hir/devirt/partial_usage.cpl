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
    start {
        {
            {
                i32t %9 = i8n 10 as i32;
                use i32t %9;
                call foo0(i32 a, i32 b, i32 c), argc args(i32t %9,i8n 10,i8n 10,);
                i16t %10 = i8n 10 as i16;
                use i16t %10;
                call foo2(i16 a, i16 b, i16 c), argc args(i16t %10,i8n 10,i8n 10,);
                i8t %11 = i8n 10 as i8;
                use i8t %11;
                call foo1(i8 a, i8 b, i8 c), argc args(i8t %11,i8n 10,i8n 10,);
            }
        }
    }
}
/:
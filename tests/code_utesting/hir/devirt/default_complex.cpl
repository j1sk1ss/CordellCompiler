function foo(i32 a, i32 b = 10);
function foo(i8 a, i32 b = 20, i32 c = 20);
function foo(i16 a, i64 b = 30, i8 c = 40, i32 d = 40);
start() {
    foo(10 as i32, 10 as i32);
    foo(10 as i8, 10 as i32);
    foo(10 as i16);
}

:/ OUTPUT
{
    start {
        {
            {
                i32t %9 = i8n 10 as i32;
                use i32t %9;
                i32t %10 = i8n 10 as i32;
                use i32t %10;
                call foo0(i32 a, i32 b), argc args(i32t %9,i32t %10,);
                i8t %11 = i8n 10 as i8;
                use i8t %11;
                i32t %12 = i8n 10 as i32;
                use i32t %12;
                call foo1(i8 a, i32 b, i32 c), argc args(i8t %11,i32t %12,i8n 20,);
                i16t %13 = i8n 10 as i16;
                use i16t %13;
                call foo2(i16 a, i64 b, i8 c, i32 d), argc args(i16t %13,i8n 30,i8n 40,i8n 40,);
            }
        }
    }
}
/:
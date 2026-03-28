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
    start {
        {
            {
                i32t %9 = num? 10 as i32;
                use i32t %9;
                call foo0(i32 a, i32 b, i32 c), argc args(i32t %9,num? 10,num? 10,);
                i8t %10 = num? 10 as i8;
                use i8t %10;
                call foo1(i8 a, i32 b, i32 c), argc args(i8t %10,num? 20,num? 20,);
                i16t %11 = num? 10 as i16;
                use i16t %11;
                call foo2(i16 a, i64 b, i8 c), argc args(i16t %11,num? 30,num? 40,);
            }
        }
    }
}
/:

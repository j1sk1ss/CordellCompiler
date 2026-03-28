function foo(i32 a, i32 b = 20);
function foo(i8 a, i32 b = 20);
start() {
    foo(10 as i32);
    foo(20 as i8);
}

:/ OUTPUT
{
    start {
        {
            {
                i32t %4 = num? 10 as i32;
                use i32t %4;
                call foo0(i32 a, i32 b), argc args(i32t %4,num? 20,);
                i8t %5 = num? 20 as i8;
                use i8t %5;
                call foo1(i8 a, i32 b), argc args(i8t %5,num? 20,);
            }
        }
    }
}
/:

function foo(i32 a, i32 b, i32 c);
function foo(i8 a, i8 b, i8 c);
function foo(i16 a, i16 b, i16 c);
start() {
    foo(10 as i32, 10 as i32, 10 as i32);
    foo(10 as i32, 10 as i8, 10 as i8);
    foo(10 as i32, 10 as i8, 10 as i16);
    foo(10 as i16, 10 as i16, 10 as i8);
    foo(10 as i32, 10 as i8, 10 as i16);
}

:/ OUTPUT
{
    start {
        {
            {
                i32t %9 = num? 10 as i32;
                use i32t %9;
                i32t %10 = num? 10 as i32;
                use i32t %10;
                i32t %11 = num? 10 as i32;
                use i32t %11;
                call foo0(i32 a, i32 b, i32 c), argc args(i32t %9,i32t %10,i32t %11,);
                i32t %12 = num? 10 as i32;
                use i32t %12;
                i8t %13 = num? 10 as i8;
                use i8t %13;
                i8t %14 = num? 10 as i8;
                use i8t %14;
                call foo1(i8 a, i8 b, i8 c), argc args(i32t %12,i8t %13,i8t %14,);
                i32t %15 = num? 10 as i32;
                use i32t %15;
                i8t %16 = num? 10 as i8;
                use i8t %16;
                i16t %17 = num? 10 as i16;
                use i16t %17;
                call foo1(i8 a, i8 b, i8 c), argc args(i32t %15,i8t %16,i16t %17,);
                i16t %18 = num? 10 as i16;
                use i16t %18;
                i16t %19 = num? 10 as i16;
                use i16t %19;
                i8t %20 = num? 10 as i8;
                use i8t %20;
                call foo2(i16 a, i16 b, i16 c), argc args(i16t %18,i16t %19,i8t %20,);
                i32t %21 = num? 10 as i32;
                use i32t %21;
                i8t %22 = num? 10 as i8;
                use i8t %22;
                i16t %23 = num? 10 as i16;
                use i16t %23;
                call foo1(i8 a, i8 b, i8 c), argc args(i32t %21,i8t %22,i16t %23,);
            }
        }
    }
}
/:

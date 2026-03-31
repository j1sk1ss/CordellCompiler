function foo(i32 a, ...);
function foo(i8 a, ...);
start() {
    foo(10 as i32, 1, 2, 3, 4, 5, 6);
    foo(10 as i8, 3, 1);
}

:/ OUTPUT
{
    start {
        {
            {
                i32t %2 = i8n 10 as i32;
                use i32t %2;
                call foo0(i32 a, ...), argc args(i32t %2,i8n 1,i8n 2,i8n 3,i8n 4,i8n 5,i8n 6,);
                i8t %3 = i8n 10 as i8;
                use i8t %3;
                call foo1(i8 a, ...), argc args(i8t %3,i8n 3,i8n 1,);
            }
        }
    }
}
/:
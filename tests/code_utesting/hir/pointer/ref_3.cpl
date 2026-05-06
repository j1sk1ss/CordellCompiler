function foo(ptr i32 a) -> ptr i32;
start() {
    i32 a;
    dref foo(ref a) = 1 as i32;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %1 = alloc;
                i32t %2 = i8n 1 as i32;
                i32t* %3 = &(i32s %1);
                use i32t* %3;
                i32t* %4 = foo0(i32t* %3);
                *(i32t* %4) = i32t %2;
            }
        }
    }
}
/:
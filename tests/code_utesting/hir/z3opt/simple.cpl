function foo();
start() {
    i32 a = 10;
    if a != 10; {
        foo();
        exit 100;
    }

    exit 1;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i32t %1 = i8n 10 as i32;
            i32s %6 = i32t %1;
            i32t %3 = i8n 10 as i32;
            i32t %2 = i32s %6 != i32t %3;
            [unused] if i32t %2, goto lb10, else goto lb12;
            lb10:
            [unused] {
                [unused] foo0();
                [unused] u8t %4 = i8n 100 as u8;
                [unused] exit u8t %4;
            }
            goto lb12;
            lb12:
            u8t %5 = i8n 1 as u8;
            exit u8t %5;
        }
    }
}
/:

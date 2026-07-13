: BUG :
function foo<T>(T a, i32 b) -> T {
    10;
}

function foo<T>(T a, i64 b) -> T {
    20;
}

start() {
    exit (foo<i32>(10, 10 as i32) + foo<i32>(10, 10 as i64)) as u8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %7 = i8n 10 as i32;
            use i32t %7;
            i32t %8 = foo3__i32(i8n 10, i32t %7);
            i64t %9 = i8n 10 as i64;
            use i64t %9;
            i32t %10 = foo3__i32(i8n 10, i64t %9);
            i32t %11 = i32t %8 + i32t %10;
            u8t %6 = i32t %11 as u8;
            exit u8t %6;
        }
    }
    fn foo3__i32(i32 a, i64 b) -> i32
    {
        i32s %5 = alloc;
        i32s %5 = load_arg();
        i64s %4 = alloc;
        i64s %4 = load_arg();
        {
            return i8n 20;
        }
    }
}
/:
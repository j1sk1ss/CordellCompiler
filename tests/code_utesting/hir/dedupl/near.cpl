function foo(i32 a, i32 b) {
    return a * b + a * b + 100 as i32;
}

function bar(i8 a, i8 b) {
    return a * b + a * b + 100 as i8;
}

function baz(i16 a, i16 b) {
    return a * b + a * b + 101 as i16;
}

start() {
    exit (bar(1, 1) + baz(1, 1) + foo(1, 1)) as u8;
}

:/ OUTPUT
{
    fn foo0(i32 a, i32 b)
    {
        i32s %0 = alloc;
        i32s %0 = load_arg();
        i32s %1 = alloc;
        i32s %1 = load_arg();
        {
            i32t %6 = i32s %0 * i32s %1;
            i32t %7 = i32s %0 * i32s %1;
            i32t %8 = i32t %6 + i32t %7;
            i32t %9 = i8n 100 as i32;
            i32t %10 = i32t %8 + i32t %9;
            return i32t %10;
        }
    }
    fn bar1(i8 a, i8 b)
    {
        i8s %2 = alloc;
        i8s %2 = load_arg();
        i8s %3 = alloc;
        i8s %3 = load_arg();
        {
            i8t %11 = i8s %2 * i8s %3;
            i8t %12 = i8s %2 * i8s %3;
            i8t %13 = i8t %11 + i8t %12;
            i8t %14 = i8n 100 as i8;
            i8t %15 = i8t %13 + i8t %14;
            return i8t %15;
        }
    }
    fn baz2(i16 a, i16 b)
    {
        i16s %4 = alloc;
        i16s %4 = load_arg();
        i16s %5 = alloc;
        i16s %5 = load_arg();
        {
            i16t %16 = i16s %4 * i16s %5;
            i16t %17 = i16s %4 * i16s %5;
            i16t %18 = i16t %16 + i16t %17;
            i16t %19 = i8n 101 as i16;
            i16t %20 = i16t %18 + i16t %19;
            return i16t %20;
        }
    }
    fn _main()
    {
        {
            i64t %22 = foo0(i8n 1, i8n 1);
            i64t %23 = baz2(i8n 1, i8n 1);
            i64t %24 = i64t %22 + i64t %23;
            i64t %25 = foo0(i8n 1, i8n 1);
            i64t %26 = i64t %24 + i64t %25;
            u8t %21 = i64t %26 as u8;
            exit u8t %21;
        }
    }
}
/:
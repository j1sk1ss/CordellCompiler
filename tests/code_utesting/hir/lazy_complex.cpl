function foo();
function bar();
function baz() {
    i32 a = (foo() || bar()) as i32;
    i32 b = (foo() && bar()) as i32;
}

:/ OUTPUT
{
    fn baz2()
    {
        {
            i32s %0 = alloc;
            i64s %3 = alloc;
            i64t %4 = foo0();
            if i64t %4, goto lb14, else goto lb15;
            lb15:
            i64t %5 = bar1();
            i64s %3 = i64t %5;
            goto lb16;
            lb14:
            i64s %3 = csu8 1;
            goto lb16;
            lb16:
            i32t %2 = i64s %3 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc;
            i64s %7 = alloc;
            i64t %8 = foo0();
            if i64t %8, goto lb31, else goto lb32;
            lb31:
            i64t %9 = bar1();
            i64s %7 = alloc;
            i64s %7 = i64t %9;
            goto lb33;
            lb32:
            i64s %7 = csu8 0;
            goto lb33;
            lb33:
            i32t %6 = i64s %7 as i32;
            i32s %1 = i32t %6;
        }
    }
}
/:
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
            i32s %0 = alloc(8);
            i64t %3 = call foo0(), argc args();
            if i64t %3, goto lb19, else goto lb20;
            lb20:
            i64t %4 = call bar1(), argc args();
            i64t %5 = i64t %4;
            goto lb21;
            lb19:
            i64t %5 = cnst? 1;
            goto lb21;
            lb21:
            i32t %2 = i64t %5 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc(8);
            i64t %7 = call foo0(), argc args();
            if i64t %7, goto lb40, else goto lb41;
            lb40:
            i64t %8 = call bar1(), argc args();
            i64t %9 = i64t %8;
            goto lb42;
            lb41:
            i64t %9 = cnst? 0;
            goto lb42;
            lb42:
            i32t %6 = i64t %9 as i32;
            i32s %1 = i32t %6;
        }
    }
}
/:

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
            if i64t %3, goto lb13, else goto lb14;
            lb14:
            i64t %4 = call bar1(), argc args();
            i64t %5 = i64t %4;
            goto lb15;
            lb13:
            i64t %5 = csu8 1;
            goto lb15;
            lb15:
            i32t %2 = i64t %5 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc(8);
            i64t %7 = call foo0(), argc args();
            if i64t %7, goto lb28, else goto lb29;
            lb28:
            i64t %8 = call bar1(), argc args();
            i64t %9 = i64t %8;
            goto lb30;
            lb29:
            i64t %9 = csu8 0;
            goto lb30;
            lb30:
            i32t %6 = i64t %9 as i32;
            i32s %1 = i32t %6;
        }
    }
}
/:
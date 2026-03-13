function foo();
function bar();
function baz() {
    i32 a = foo() || bar();
    i32 b = foo() && bar();
}

: OUTPUT
{
    fn baz2()
    {
        {
            i32s %0 = alloc(8);
            i64t %2 = call foo0(), argc args();
            if i64t %2, goto lb8, else goto lb9;
            lb9:
            i64t %3 = call bar1(), argc args();
            i64t %4 = i64t %3;
            goto lb10;
            lb8:
            i64t %4 = cnst? 1;
            goto lb10;
            lb10:
            i32t %5 = i64t %4 as i32;
            i32s %0 = i32t %5;
            i32s %1 = alloc(8);
            i64t %6 = call foo0(), argc args();
            if i64t %6, goto lb22, else goto lb23;
            lb22:
            i64t %7 = call bar1(), argc args();
            i64t %8 = i64t %7;
            goto lb24;
            lb23:
            i64t %8 = cnst? 0;
            goto lb24;
            lb24:
            i32t %9 = i64t %8 as i32;
            i32s %1 = i32t %9;
        }
    }
}
:
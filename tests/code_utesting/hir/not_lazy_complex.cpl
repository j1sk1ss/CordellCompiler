function foo();
function bar();
function baz() {
    i32 a = @[not_lazy] (foo() || bar());
    i32 b = @[not_lazy] (foo() && bar());
}

: OUTPUT
{
    fn baz2()
    {
        {
            i32s %0 = alloc(8);
            i64t %2 = call foo0(), argc args();
            i64t %3 = call bar1(), argc args();
            if i64t %2, goto lb12, else goto lb13;
            lb13:
            i64t %4 = i64t %3;
            goto lb14;
            lb12:
            i64t %4 = cnst? 1;
            goto lb14;
            lb14:
            i32t %5 = i64t %4 as i32;
            i32s %0 = i32t %5;
            i32s %1 = alloc(8);
            i64t %6 = call foo0(), argc args();
            i64t %7 = call bar1(), argc args();
            if i64t %6, goto lb26, else goto lb27;
            lb26:
            i64t %8 = i64t %7;
            goto lb28;
            lb27:
            i64t %8 = cnst? 0;
            goto lb28;
            lb28:
            i32t %9 = i64t %8 as i32;
            i32s %1 = i32t %9;
        }
    }
}
:
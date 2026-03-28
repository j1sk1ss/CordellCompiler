function foo();
function bar();
function baz() {
    i32 a = (@[not_lazy] (foo() || bar())) as i32;
    i32 b = (@[not_lazy] (foo() && bar())) as i32;
}

: OUTPUT
{
    fn baz2()
    {
        {
            i32s %0 = alloc(8);
            i64t %3 = call foo0(), argc args();
            i64t %4 = call bar1(), argc args();
            if i64t %3, goto lb28, else goto lb29;
            lb29:
            i64t %5 = i64t %4;
            goto lb30;
            lb28:
            i64t %5 = cnst? 1;
            goto lb30;
            lb30:
            i32t %2 = i64t %5 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc(8);
            i64t %7 = call foo0(), argc args();
            i64t %8 = call bar1(), argc args();
            if i64t %7, goto lb53, else goto lb54;
            lb53:
            i64t %9 = i64t %8;
            goto lb55;
            lb54:
            i64t %9 = cnst? 0;
            goto lb55;
            lb55:
            i32t %6 = i64t %9 as i32;
            i32s %1 = i32t %6;
        }
    }
}
:
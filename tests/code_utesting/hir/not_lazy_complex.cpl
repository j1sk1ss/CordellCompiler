function foo();
function bar();
function baz() {
    i32 a = (@[not_lazy] (foo() || bar())) as i32;
    i32 b = (@[not_lazy] (foo() && bar())) as i32;
}

:/ OUTPUT
{
    fn baz2()
    {
        {
            i32s %0 = alloc(8);
            i64t %3 = call foo0(), argc args();
            i64t %4 = call bar1(), argc args();
            if i64t %3, goto lb24, else goto lb25;
            lb25:
            i64t %5 = i64t %4;
            goto lb26;
            lb24:
            i64t %5 = cnst? 1;
            goto lb26;
            lb26:
            i32t %2 = i64t %5 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc(8);
            i64t %7 = call foo0(), argc args();
            i64t %8 = call bar1(), argc args();
            if i64t %7, goto lb45, else goto lb46;
            lb45:
            i64t %9 = i64t %8;
            goto lb47;
            lb46:
            i64t %9 = cnst? 0;
            goto lb47;
            lb47:
            i32t %6 = i64t %9 as i32;
            i32s %1 = i32t %6;
        }
    }
}
/:

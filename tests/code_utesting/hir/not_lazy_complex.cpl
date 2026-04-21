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
            if i64t %3, goto lb17, else goto lb18;
            lb18:
            i64t %5 = i64t %4;
            goto lb19;
            lb17:
            i64t %5 = csu8 1;
            goto lb19;
            lb19:
            i32t %2 = i64t %5 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc(8);
            i64t %7 = call foo0(), argc args();
            i64t %8 = call bar1(), argc args();
            if i64t %7, goto lb32, else goto lb33;
            lb32:
            i64t %9 = i64t %8;
            goto lb34;
            lb33:
            i64t %9 = csu8 0;
            goto lb34;
            lb34:
            i32t %6 = i64t %9 as i32;
            i32s %1 = i32t %6;
        }
    }
}
/:
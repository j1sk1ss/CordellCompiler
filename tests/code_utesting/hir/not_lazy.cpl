function foo() {
    i32 a;
    i32 b;
    i32 c = @[not_lazy] (a || b);
    i32 d = @[not_lazy] (a && b);
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            i32s %0 = alloc;
            i32s %1 = alloc;
            i32s %2 = alloc;
            if i32s %0, goto lb15, else goto lb16;
            lb16:
            i32t %4 = i32s %1;
            goto lb17;
            lb15:
            i32t %4 = csu8 1;
            goto lb17;
            lb17:
            i32s %2 = i32t %4;
            i32s %3 = alloc;
            if i32s %0, goto lb27, else goto lb28;
            lb27:
            i32t %5 = i32s %1;
            goto lb29;
            lb28:
            i32t %5 = csu8 0;
            goto lb29;
            lb29:
            i32s %3 = i32t %5;
        }
    }
}
/:
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
            i32s %0 = alloc(8);
            i32s %1 = alloc(8);
            i32s %2 = alloc(8);
            if i32s %0, goto lb15, else goto lb16;
            lb16:
            i32t %4 = i32s %1;
            goto lb17;
            lb15:
            i32t %4 = csu8 1;
            goto lb17;
            lb17:
            i32s %2 = i32t %4;
            i32s %3 = alloc(8);
            if i32s %0, goto lb25, else goto lb26;
            lb25:
            i32t %5 = i32s %1;
            goto lb27;
            lb26:
            i32t %5 = csu8 0;
            goto lb27;
            lb27:
            i32s %3 = i32t %5;
        }
    }
}
/:
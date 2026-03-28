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
            if i32s %0, goto lb{X} else goto lb{X}
            lb{X}
            i32t %4 = i32s %1;
            goto lb{X}
            lb{X}
            i32t %4 = cnst? 1;
            goto lb{X}
            lb{X}
            i32s %2 = i32t %4;
            i32s %3 = alloc(8);
            if i32s %0, goto lb{X} else goto lb{X}
            lb{X}
            i32t %5 = i32s %1;
            goto lb{X}
            lb{X}
            i32t %5 = cnst? 0;
            goto lb{X}
            lb{X}
            i32s %3 = i32t %5;
        }
    }
}
/:

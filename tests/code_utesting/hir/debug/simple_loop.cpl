function foo() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
setpos, line=1, column=10, file=<unknown>
{
setpos, line=1, column=10, file=<unknown>
setpos, line=1, column=10, file=<unknown>
    fn foo0()
    {
setpos, line=2, column=7, file=<unknown>
        {
setpos, line=2, column=32, file=<unknown>
setpos, line=2, column=32, file=<unknown>
            i64s %0 = alloc(8);
            i64s %0 = cnst? 10;
            lb{X}
            goto lb{X}
            lb{X}
            {
                i64t %1 = i64s %0 - cnst? 1;
                i64s %0 = i64t %1;
                if i64t %1, goto lb{X} else goto lb{X}
            }
            lb{X}
        }
    }
}
/:

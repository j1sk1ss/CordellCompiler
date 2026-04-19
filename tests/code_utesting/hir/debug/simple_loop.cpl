function foo() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
setpos, line=2, column=86, file={X}
{
setpos, line=1, column=10, file={X}
setpos, line=1, column=10, file={X}
    fn foo0()
    {
setpos, line=2, column=7, file={X}
        {
setpos, line=2, column=32, file={X}
setpos, line=2, column=32, file={X}
            i64s %0 = alloc(8);
            i64s %0 = cnst? 10;
            lb12:
            goto lb13;
            lb13:
            {
                i64t %1 = i64s %0 - cnst? 1;
                i64s %0 = i64t %1;
                if i64s %0, goto lb12, else goto lb14;
            }
            lb14:
        }
    }
}
/:
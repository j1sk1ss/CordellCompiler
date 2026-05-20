function foo() {
    return 1;
}

function bar() {
    return foo();
}

function baz() {
    return bar();
}

start() {
    exit baz() as u8;
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            return i8n 1;
        }
    }
    fn bar1()
    {
        {
            i64t %0 = i8n 1;
            goto lb42;
            lb42:
            [unused] i64t %0 = foo0();
            return i64t %0;
        }
    }
    fn baz2()
    {
        {
            i64t %0 = i8n 1;
            goto lb54;
            lb54:
            i64t %1 = i64t %0;
            goto lb47;
            lb47:
            [unused] i64t %1 = bar1();
            return i64t %1;
        }
    }
    fn _main()
    {
        {
            i64t %0 = i8n 1;
            goto lb64;
            lb64:
            i64t %1 = i64t %0;
            goto lb65;
            lb65:
            i64t %3 = i64t %1;
            goto lb55;
            lb55:
            [unused] i64t %3 = baz2();
            u8t %2 = i64t %3 as u8;
            exit u8t %2;
        }
    }
}
/:
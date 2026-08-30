function bar();
function baz();
@[inline(never)]
function foo(i8 a) -> i0 {
    if a != 0; bar();
    else       baz();
}

start() {
    foo(0);
    exit 0;
}

:/ OUTPUT
{
    fn foo2(i8 a) -> i0
    {
        i8s %0 = alloc;
        i8s %3 = load_arg();
        {
            i8t %1 = i8s %3 != i8n 0;
            if i8t %1, goto lb9, else goto lb10;
            lb9:
            {
                bar0();
            }
            goto lb11;
            lb10:
            {
                baz1();
            }
            goto lb11;
            lb11:
        }
    }
    fn _main()
    {
        {
            foo2(i8n 0);
            u8t %2 = i8n 0 as u8;
            exit u8t %2;
        }
    }
}
/:
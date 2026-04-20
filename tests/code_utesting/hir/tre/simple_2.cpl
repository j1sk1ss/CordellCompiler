@[entry]
function foo(i32 a) {
    foo(a + 1);
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            lb20:
            i32t %2 = i8n 1 as i32;
            i32t %1 = i32s %0 + i32t %2;
            use i32t %1;
            i32s %0 = i32t %1;
            goto lb20;
            [unused] call _main(i32 a), argc args(i32t %1,);
        }
    }
}
/:
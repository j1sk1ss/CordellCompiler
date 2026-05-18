@[entry]
function bar(i32 a);
function bar(i32 a) {}
function foo() -> i32 {
    return 0;
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %1 = alloc;
        i32s %1 = load_starg();
        {
        }
    }
    fn foo1() -> i32
    {
        {
            i32t %2 = i8n 0 as i32;
            return i32t %2;
        }
    }
}
/:
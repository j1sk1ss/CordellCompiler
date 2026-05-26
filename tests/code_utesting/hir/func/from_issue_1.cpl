#define NULL 0 as ptr i0
function malloc(i64 size) -> ptr i0 {
    return NULL;
}

:/ OUTPUT
{
    fn malloc0(i64 size) -> i0*
    {
        i64s %0 = alloc;
        i64s %0 = load_arg();
        {
            i0t* %1 = i8n 0 as *ptr;
            return i0t* %1;
        }
    }
}
/:
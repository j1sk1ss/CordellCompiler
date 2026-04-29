function unsafe(ptr i32 a) -> i0 {
    dref a = 1;
}

function safe(ptr i32 a) -> i0 {
    if a; {
        dref a = 1;
    }
}

start() {
    unsafe(0);
    safe(0);
}

:/ OUTPUT
{
    fn unsafe0(i32* a) -> i0
    {
        i32s* %0 = alloc;
        i32s* %4 = load_arg();
        {
            i32t %2 = i8n 1 as i32;
            *(i32s* %4) = i32t %2;
        }
    }
    fn safe1(i32* a) -> i0
    {
        i32s* %1 = alloc;
        i32s* %5 = load_arg();
        {
            if i32s* %5, goto lb23, else goto lb25;
            lb23:
            {
                i32t %3 = i8n 1 as i32;
                *(i32s* %5) = i32t %3;
            }
            goto lb25;
            lb25:
        }
    }
    start {
        {
            {
                unsafe0(i8n 0);
                safe1(i8n 0);
            }
        }
    }
}
/:
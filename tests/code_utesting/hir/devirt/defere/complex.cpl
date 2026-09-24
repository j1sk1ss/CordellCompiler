function free(ptr i0 p) -> i0;
function malloc(i32 s) -> ptr i0;

start(i32 a) {
    ptr i0 data = malloc(100 as i32);
    defer free(data);
    if a == 1; exit 1;
    else if a == 2; exit 2;
    exit 3;
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %2 = alloc;
        i32s %2 = load_starg();
        {
            i0s* %3 = alloc;
            i32t %4 = i8n 100 as i32;
            use i32t %4;
            i0t* %5 = malloc1(i32t %4);
            i0s* %3 = i0t* %5;
            [unused] defer {
                [unused] use i0s* %3;
                [unused] free0(i0s* %3);
            [unused] }
            i32t %7 = i8n 1 as i32;
            i32t %6 = i32s %2 == i32t %7;
            if i32t %6, goto lb22, else goto lb23;
            lb22:
            {
                u8t %8 = i8n 1 as u8;
                use i0s* %3;
                free0(i0s* %3);
                exit u8t %8;
            }
            goto lb24;
            lb23:
            {
                i32t %10 = i8n 2 as i32;
                i32t %9 = i32s %2 == i32t %10;
                if i32t %9, goto lb32, else goto lb34;
                lb32:
                {
                    u8t %11 = i8n 2 as u8;
                    use i0s* %3;
                    free0(i0s* %3);
                    exit u8t %11;
                }
                goto lb34;
                lb34:
            }
            goto lb24;
            lb24:
            u8t %12 = i8n 3 as u8;
            use i0s* %3;
            free0(i0s* %3);
            exit u8t %12;
        }
    }
}
/:
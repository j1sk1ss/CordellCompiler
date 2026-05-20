function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    str msg = "Hello, World!\n";
    syscall(0x2000004, 1, ref msg, strlen(ref msg));
}

:/ OUTPUT
{
    fn strlen0(i8* s) -> i32
    {
        i8s* %0 = alloc;
        i8s* %0 = load_arg();
        {
            i32s %1 = alloc;
            i32t %5 = i8n 0 as i32;
            i32s %1 = i32t %5;
            lb16:
            i8t %6 = *(i8s* %0);
            if i8t %6, goto lb17, else goto lb18;
            lb17:
            {
                i32t %8 = i8n 1 as i32;
                i32t %7 = i32s %1 + i32t %8;
                i32s %1 = i32t %7;
                i8t* %10 = i8n 1 as *ptr;
                i8t* %9 = i8s* %0 + i8t* %10;
                i8s* %0 = i8t* %9;
            }
            goto lb16;
            lb18:
            return i32s %1;
        }
    }
    fn _main(i32 argc, i8** argv)
    {
        i32s %2 = alloc;
        i32s %2 = load_starg();
        i8s** %3 = alloc;
        i8s** %3 = load_starg();
        {
            strs %4 = str_alloc(Hello, World!
);
            i8t* %11 = &(strs %4);
            use i8t* %11;
            i8t* %12 = &(strs %4);
            use i8t* %12;
            i32t %13 = strlen0(i8t* %12);
            use i32t %13;
            syscall(i32n 33554436, i8n 1, i8t* %11, i32t %13);
        }
    }
}
/:
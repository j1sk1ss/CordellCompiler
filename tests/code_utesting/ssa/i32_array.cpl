function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

glob arr a[16, i32];

start(i32 argc, ptr ptr i8 argv) {
    a[2] = 7;
    a[7] = 3;
    putdigit(a[7]);
    putdigit(a[2]);
    exit 0;
}

:/ OUTPUT
{
    fn putc0(i8 c) -> i0
    {
        i8s %0 = alloc(8);
        i8s %30 = load_arg();
        {
            i8t* %6 = &(i8s %30);
            use i8t* %6;
            syscall, argc: args(i32n 33554436,i8n 1,i8t* %6,i8n 1,);
        }
    }
    fn putdigit1(i32 x) -> i0
    {
        i32s %1 = alloc(8);
        i32s %31 = load_arg();
        {
            i8s %2 = alloc(8);
            i8t %7 = i32s %31 as i8;
            i8s %32 = i8t %7;
            i8t %8 = i8n 48 as i8;
            i8t %9 = i8s %32 + i8t %8;
            i8s %33 = i8t %9;
            use i8s %33;
            call putc0(i8 c) -> i0, argc args(i8s %33,);
        }
    }
    arrg %3 = arr_alloc(i8n 16);
    start {
        {
            i32s %4 = alloc(8);
            i32s %34 = load_starg();
            i8s** %5 = alloc(8);
            i8s** %35 = load_starg();
            {
                i32t* %10 = &(arrg %3);
                i8t %11 = i8n 2 * csu8 4;
                i32t* %13 = i8t %11 as *ptr;
                i32t* %12 = i32t* %10 + i32t* %13;
                i32t %14 = i8n 7 as i32;
                *(i32t* %12) = i32t %14;
                i32t* %15 = &(arrg %3);
                i8t %16 = i8n 7 * csu8 4;
                i32t* %18 = i8t %16 as *ptr;
                i32t* %17 = i32t* %15 + i32t* %18;
                i32t %19 = i8n 3 as i32;
                *(i32t* %17) = i32t %19;
                i32t* %20 = &(arrg %3);
                i8t %21 = i8n 7 * csu8 4;
                i32t* %23 = i8t %21 as *ptr;
                i32t* %22 = i32t* %20 + i32t* %23;
                i32t %24 = *(i32t* %22);
                use i32t %24;
                call putdigit1(i32 x) -> i0, argc args(i32t %24,);
                i32t* %25 = &(arrg %3);
                i8t %26 = i8n 2 * csu8 4;
                i32t* %28 = i8t %26 as *ptr;
                i32t* %27 = i32t* %25 + i32t* %28;
                i32t %29 = *(i32t* %27);
                use i32t %29;
                call putdigit1(i32 x) -> i0, argc args(i32t %29,);
                exit i8n 0;
            }
        }
    [unused] }
}
/:
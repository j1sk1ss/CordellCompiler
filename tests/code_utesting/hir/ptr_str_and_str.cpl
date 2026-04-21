function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    ptr i8 a = ref "AB";
    str b = "CD";

    putc(a[0]);
    putc(a[1]);
    putc(b[0]);
    putc(b[1]);
    exit 0;
}

:/ OUTPUT
{
    fn putc0(i8 c) -> i0
    {
        i8s %0 = alloc(8);
        i8s %0 = load_arg();
        {
            i8t* %3 = &(i8s %0);
            use i8t* %3;
            syscall, argc: args(i32n 33554436,i8n 1,i8t* %3,i8n 1,);
        }
    }
    start {
        {
            {
                i8s* %1 = alloc(8);
                i64t* %4 = &(AB);
                i8t* %5 = i64t* %4 as *ptr;
                i8s* %1 = i8t* %5;
                strs %2 = str_alloc(CD);
                i8t* %6 = i8s* %1;
                i8t %7 = i8n 0 * csu8 1;
                i8t* %9 = i8t %7 as *ptr;
                i8t* %8 = i8t* %6 + i8t* %9;
                i8t %10 = *(i8t* %8);
                use i8t %10;
                call putc0(i8 c) -> i0, argc args(i8t %10,);
                i8t* %11 = i8s* %1;
                i8t %12 = i8n 1 * csu8 1;
                i8t* %14 = i8t %12 as *ptr;
                i8t* %13 = i8t* %11 + i8t* %14;
                i8t %15 = *(i8t* %13);
                use i8t %15;
                call putc0(i8 c) -> i0, argc args(i8t %15,);
                i8t* %16 = &(strs %2);
                i8t %17 = i8n 0 * csu8 1;
                i8t* %19 = i8t %17 as *ptr;
                i8t* %18 = i8t* %16 + i8t* %19;
                i8t %20 = *(i8t* %18);
                use i8t %20;
                call putc0(i8 c) -> i0, argc args(i8t %20,);
                i8t* %21 = &(strs %2);
                i8t %22 = i8n 1 * csu8 1;
                i8t* %24 = i8t %22 as *ptr;
                i8t* %23 = i8t* %21 + i8t* %24;
                i8t %25 = *(i8t* %23);
                use i8t %25;
                call putc0(i8 c) -> i0, argc args(i8t %25,);
                exit i8n 0;
            }
        }
    }
}
/:
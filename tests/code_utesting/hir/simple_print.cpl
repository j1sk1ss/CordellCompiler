start(i32 argc, ptr ptr i8 argv) {
    syscall(0x2000004, 1, argv[1 as i32], 12);
}

:/ OUTPUT
{
    start {
        {
            i32s %0 = alloc(8);
            i32s %0 = load_starg();
            i8s** %1 = alloc(8);
            i8s** %1 = load_starg();
            {
                i32t %2 = i8n 1 as i32;
                i8t** %3 = i8s** %1;
                i32t %4 = i32t %2 * csu8 8;
                i8t** %6 = i32t %4 as *ptr;
                i8t** %5 = i8t** %3 + i8t** %6;
                i8t* %7 = *(i8t** %5);
                use i8t* %7;
                syscall, argc: args(i32n 33554436,i8n 1,i8t* %7,i8n 12,);
            }
        }
    }
}
/:
container game {
    arr actions[2, ptr i0];
    i32 value;
}

function inc(ptr game g) -> i32 {
    g.value += 1;
    return g.value;
}

function run(ptr game g, i32 index) -> i32 {
    return g.actions[index](g) as i32;
}

start() {
    game g;
    g.value = 41 as i32;
    g.actions[0] = inc;

    exit run(ref g, 0 as i32) as u8;
}

:/ OUTPUT
{
    fn inc0(u8* g) -> i32
    {
        u8s* %2 = alloc;
        u8s* %2 = load_arg();
        {
            u8t* %6 = u8s* %2 + csu8 16;
            i32t %7 = *(u8t* %6);
            i32t %8 = i8n 1 as i32;
            i32t %9 = i32t %7 + i32t %8;
            u8t* %10 = u8s* %2 + csu8 16;
            i32t* %11 = u8t* %10 as *ptr;
            *(i32t* %11) = i32t %9;
            u8t* %12 = u8s* %2 + csu8 16;
            i32t %13 = *(u8t* %12);
            return i32t %13;
        }
    }
    fn run1(u8* g, i32 index) -> i32
    {
        u8s* %3 = alloc;
        u8s* %3 = load_arg();
        i32s %4 = alloc;
        i32s %4 = load_arg();
        {
            u8t* %15 = u8s* %3 + csu8 0;
            i0t** %16 = u8t* %15 as *ptr;
            i0t** %17 = i0t** %16;
            i32t %18 = i32s %4 * csu8 8;
            i0t** %20 = i32t %18 as *ptr;
            i0t** %19 = i0t** %17 + i0t** %20;
            i0t* %21 = *(i0t** %19);
            use u8s* %3;
            i64t %22 = i0t* %21(u8s* %3);
            i32t %14 = i64t %22 as i32;
            return i32t %14;
        }
    }
    fn _main()
    {
        {
            u8s %5 = arr_alloc(csu8 24);
            i32t %23 = i8n 41 as i32;
            u8t* %24 = &(u8s %5);
            u8t* %25 = u8t* %24 + csu8 16;
            i32t* %26 = u8t* %25 as *ptr;
            *(i32t* %26) = i32t %23;
            i0t* %27 = &(inc0(u8* g) -> i32);
            u8t* %28 = &(u8s %5);
            u8t* %29 = u8t* %28 + csu8 0;
            i0t** %30 = u8t* %29 as *ptr;
            i0t** %31 = i0t** %30;
            i8t %32 = i8n 0 * csu8 8;
            i0t** %34 = i8t %32 as *ptr;
            i0t** %33 = i0t** %31 + i0t** %34;
            *(i0t** %33) = i0t* %27;
            u8t* %36 = &(u8s %5);
            use u8t* %36;
            i32t %37 = i8n 0 as i32;
            use i32t %37;
            i32t %38 = run1(u8t* %36, i32t %37);
            u8t %35 = i32t %38 as u8;
            exit u8t %35;
        }
    }
}
/:
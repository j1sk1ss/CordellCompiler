container cell {
    i32 value;
}

container table {
    arr cells[2, cell];

    @[self]
    function get(ptr table self, i32 index) -> ptr cell;
}

function table::get(ptr table self, i32 index) -> ptr cell {
    return self.cells[index]; :/ TODO: ref self.cells[index] is a correct way to return a reference to an array element /:
}

start() {
    table t;
    t.cells[0].value = 42 as i32;
    t.cells[1].value = 7 as i32;

    ptr cell c = t.get(0 as i32);
    exit c.value as u8;
}

:/ OUTPUT
{
    fn get0(u8* self, i32 index) -> u8*
    {
        u8s* %4 = alloc;
        u8s* %4 = load_arg();
        i32s %5 = alloc;
        i32s %5 = load_arg();
        {
            u8t* %8 = u8s* %4 + csu8 0;
            i8t* %9 = u8t* %8 as *ptr;
            i8t* %10 = i8t* %9;
            i32t %11 = i32s %5 * csu8 8;
            i8t* %13 = i32t %11 as *ptr;
            i8t* %12 = i8t* %10 + i8t* %13;
            u8t* %14 = i8t* %12 as *ptr;
            return u8t* %14;
        }
    }
    fn _main()
    {
        {
            u8s %6 = arr_alloc(csu8 16);
            i32t %15 = i8n 42 as i32;
            u8t* %16 = &(u8s %6);
            u8t* %17 = u8t* %16 + csu8 0;
            i8t* %18 = u8t* %17 as *ptr;
            i8t* %19 = i8t* %18;
            i8t %20 = i8n 0 * csu8 8;
            i8t* %22 = i8t %20 as *ptr;
            i8t* %21 = i8t* %19 + i8t* %22;
            u8t* %23 = i8t* %21 + csu8 0;
            i32t* %24 = u8t* %23 as *ptr;
            *(i32t* %24) = i32t %15;
            i32t %25 = i8n 7 as i32;
            u8t* %26 = &(u8s %6);
            u8t* %27 = u8t* %26 + csu8 0;
            i8t* %28 = u8t* %27 as *ptr;
            i8t* %29 = i8t* %28;
            i8t %30 = i8n 1 * csu8 8;
            i8t* %32 = i8t %30 as *ptr;
            i8t* %31 = i8t* %29 + i8t* %32;
            u8t* %33 = i8t* %31 + csu8 0;
            i32t* %34 = u8t* %33 as *ptr;
            *(i32t* %34) = i32t %25;
            u8s* %7 = alloc;
            u8t* %35 = &(u8s %6);
            use u8t* %35;
            i32t %36 = i8n 0 as i32;
            use i32t %36;
            u8t* %37 = get0(u8t* %35, i32t %36);
            u8s* %7 = u8t* %37;
            u8t* %39 = u8s* %7 + csu8 0;
            i32t %40 = *(u8t* %39);
            u8t %38 = i32t %40 as u8;
            exit u8t %38;
        }
    }
}
/:
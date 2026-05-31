container node {
    i16 a;
    i8 b;

    @[self]
    function init(ptr node self) -> i0 {
        self.a = 0;
        self.b = 0;
    }
}

start() {
    arr nodes[10, node];
    nodes[0].init();
    i32 a = nodes[0].a;
}

:/ OUTPUT
{
    fn init0(u8* self) -> i0
    {
        u8s* %2 = alloc;
        u8s* %2 = load_arg();
        {
            u8t* %5 = u8s* %2 + csu8 0;
            i16t* %6 = u8t* %5;
            i16t %7 = i8n 0 as i16;
            *(i16t* %6) = i16t %7;
            u8t* %8 = u8s* %2 + csu8 8;
            i8t* %9 = u8t* %8;
            *(i8t* %9) = i8n 0;
        }
    }
    fn _main()
    {
        {
            arrs %3 = arr_alloc(csi8 160);
            i8t* %10 = &(arrs %3);
            i8t %11 = i8n 0 * csu8 16;
            i8t* %13 = i8t %11 as *ptr;
            i8t* %12 = i8t* %10 + i8t* %13;
            use i8t* %12;
            init0(i8t* %12);
            i32s %4 = alloc;
            i8t* %14 = &(arrs %3);
            i8t %15 = i8n 0 * csu8 16;
            i8t* %17 = i8t %15 as *ptr;
            i8t* %16 = i8t* %14 + i8t* %17;
            u8t* %18 = i8t* %16 + csu8 0;
            i16t %19 = *(u8t* %18);
            i32t %20 = i16t %19 as i32;
            i32s %4 = i32t %20;
        }
    }
}
/:
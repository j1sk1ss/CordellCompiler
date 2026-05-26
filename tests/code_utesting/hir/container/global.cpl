container io {
    i32 fd;

    @[self]
    function init(ptr io self, i32 fd) -> i0 {
        self.fd = fd;
    }

    @[self]
    function open(ptr io self) -> i32 {
        return self.fd;
    }
}

glob io instance;

start() {
    instance.init(-1);
    instance.open();
}

:/ OUTPUT
{
    fn open1(u8* self) -> i32
    {
        u8s* %3 = alloc;
        u8s* %3 = load_arg();
        {
            u8t* %5 = u8s* %3 + csu8 0;
            i32t %6 = *(u8t* %5);
            return i32t %6;
        }
    }
    fn init0(u8* self, i32 fd) -> i0
    {
        u8s* %1 = alloc;
        u8s* %1 = load_arg();
        i32s %2 = alloc;
        i32s %2 = load_arg();
        {
            u8t* %7 = u8s* %1 + csu8 0;
            i32t* %8 = u8t* %7;
            *(i32t* %8) = i32s %2;
        }
    }
    u8g %4 = arr_alloc(csu8 32);
    fn _main()
    {
        {
            u8t* %9 = &(u8g %4);
            use u8t* %9;
            init0(u8t* %9, i8n -1);
            u8t* %10 = &(u8g %4);
            use u8t* %10;
            open1(u8t* %10);
        }
    }
}
/:
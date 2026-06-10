function malloc(i32 size) -> ptr i0;
function strdup(ptr i8 msg) -> ptr i8;
function free(ptr i0 pointer) -> i0;

container string {
    function new() -> ptr string {
        return malloc(sizeof(string));
    }

    @[self]
    function destroy(ptr string self) -> i0 {
        free(self);
    }
}

container node {
    function new() -> ptr node {
        return malloc(sizeof(node));
    }
}

start() {
    ptr string message = string::new();
    message.destroy();
    ptr node nd = node::new();
}

:/ OUTPUT
{
    fn destroy4(u8* self) -> i0
    {
        u8s* %3 = alloc;
        u8s* %3 = load_arg();
        {
            use u8s* %3;
            free2(u8s* %3);
        }
    }
    fn new3() -> u8*
    {
        {
            i0t* %6 = malloc0(csu8 0);
            u8t* %7 = i0t* %6 as *ptr;
            return u8t* %7;
        }
    }
    fn new5() -> u8*
    {
        {
            i0t* %8 = malloc0(csu8 0);
            u8t* %9 = i0t* %8 as *ptr;
            return u8t* %9;
        }
    }
    fn _main()
    {
        {
            u8s* %4 = alloc;
            u8t* %10 = new3();
            u8s* %4 = u8t* %10;
            use u8s* %4;
            destroy4(u8s* %4);
            u8s* %5 = alloc;
            u8t* %11 = new5();
            u8s* %5 = u8t* %11;
        }
    }
}
/:
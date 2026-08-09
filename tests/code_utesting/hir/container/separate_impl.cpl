container node {
    function new() -> ptr node;
    @[self] 
    function destroy(ptr node self) -> i0;
}

function node::new() -> ptr node {
    0;
}

function node::destroy(ptr node self) -> i0 {
}

start() {
    ptr node nd = node::new();
    nd.destroy();
}

:/ OUTPUT
{
    fn new0() -> u8*
    {
        {
            use i8n 0;
        }
    }
    fn destroy1(u8* self) -> i0
    {
        u8s* %1 = alloc;
        u8s* %1 = load_arg();
        {
        }
    }
    fn _main()
    {
        {
            u8s* %2 = alloc;
            u8t* %3 = new0();
            u8s* %2 = u8t* %3;
            use u8s* %2;
            destroy1(u8s* %2);
        }
    }
}
/:
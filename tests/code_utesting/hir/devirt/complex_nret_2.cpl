container file {
    ptr i0 body;
    i8     owned;

    @[self]
    function init(ptr file self, ptr i0 body, i8 owned) -> i0;
    function wrap(ptr i0 body, i8 owned) -> ptr file;
}

function malloc(i32 s) -> ptr i0;

function file::init(ptr file self, ptr i0 body, i8 owned) -> i0 {
    if not self; return;
    self.body  = body;
    self.owned = owned;
}

function file::wrap(ptr i0 body, i8 owned) -> ptr file {
    if not body; return 0 as ptr file;
    ptr file self = malloc(sizeof(file)) as ptr file;
    if not self; return 0 as ptr file;
    self.init(body, owned);
    self
}

:/ OUTPUT
{
    fn init0(u8* self, i0* body, i8 owned) -> i0
    {
        u8s* %8 = alloc;
        u8s* %8 = load_arg();
        i0s* %9 = alloc;
        i0s* %9 = load_arg();
        i8s %10 = alloc;
        i8s %10 = load_arg();
        {
            u8t %14 = not u8s* %8;
            if u8t %14, goto lb20, else goto lb22;
            lb20:
            {
                return ;
            }
            goto lb22;
            lb22:
            u8t** %15 = u8s* %8 + csu8 0;
            i0t** %16 = u8t** %15 as *ptr;
            *(i0t** %16) = i0s* %9;
            u8t* %17 = u8s* %8 + csu8 8;
            i8t* %18 = u8t* %17 as *ptr;
            *(i8t* %18) = i8s %10;
        }
    }
    fn wrap1(i0* body, i8 owned) -> u8*
    {
        i0s* %11 = alloc;
        i0s* %11 = load_arg();
        i8s %12 = alloc;
        i8s %12 = load_arg();
        {
            i0t %19 = not i0s* %11;
            if i0t %19, goto lb54, else goto lb56;
            lb54:
            {
                u8t* %20 = i8n 0 as *ptr;
                return u8t* %20;
            }
            goto lb56;
            lb56:
            u8s* %13 = alloc;
            i0t* %22 = malloc2(csu8 16);
            u8t* %21 = i0t* %22 as *ptr;
            u8s* %13 = u8t* %21;
            u8t %23 = not u8s* %13;
            if u8t %23, goto lb72, else goto lb74;
            lb72:
            {
                u8t* %24 = i8n 0 as *ptr;
                return u8t* %24;
            }
            goto lb74;
            lb74:
            use u8s* %13;
            use i0s* %11;
            use i8s %12;
            init0(u8s* %13, i0s* %11, i8s %12);
            return u8s* %13;
        }
    }
}
/:
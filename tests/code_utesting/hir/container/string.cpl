container string {
    ptr i8 body;

    @[self]
    function new(ptr string self, ptr i8 data) -> i0 {
        self.body = data;
    }

    @[self]
    function strlen(ptr string self) -> i32 {
        i32 index = 0;
        ptr i8 s = self.body;
        while dref s; {
            index += 1;
            s += 1;
        }

        return index;
    }

    @[self]
    function equals(ptr string self, ptr string other) -> i0 {
        ptr i8 s1 = self.body;
        ptr i8 s2 = other.body;

        while dref s1 && dref s2; {
            if dref s1 != dref s2; return 0;
            s1 += 1;
            s2 += 1;
        }

        return not dref s1 && not dref s2;
    }
}

start() {
    string msg;
    msg.new("Hello, World!");
    string other;
    other.new("Hello, World!");
    if msg.equals(other); exit 1;
    exit 0;
}

:/ OUTPUT
{
    fn equals2(u8* self, u8* other) -> i0
    {
        u8s* %6 = alloc;
        u8s* %6 = load_arg();
        u8s* %7 = alloc;
        u8s* %7 = load_arg();
        {
            i8s* %8 = alloc;
            u8t* %12 = u8s* %6 + csu8 0;
            i8t* %13 = *(u8t* %12);
            i8s* %8 = i8t* %13;
            i8s* %9 = alloc;
            u8t* %14 = u8s* %7 + csu8 0;
            i8t* %15 = *(u8t* %14);
            i8s* %9 = i8t* %15;
            lb29:
            i64s %16 = alloc;
            i8t %17 = *(i8s* %8);
            if i8t %17, goto lb35, else goto lb36;
            lb35:
            i8t %18 = *(i8s* %9);
            i64s %16 = alloc;
            i64t %19 = i8t %18 as i64;
            i64s %16 = i64t %19;
            goto lb37;
            lb36:
            i64s %16 = csu8 0;
            goto lb37;
            lb37:
            if i64s %16, goto lb30, else goto lb31;
            lb30:
            {
                i8t %20 = *(i8s* %8);
                i8t %21 = *(i8s* %9);
                i8t %22 = i8t %20 != i8t %21;
                if i8t %22, goto lb46, else goto lb48;
                lb46:
                {
                    i0t %23 = i8n 0;
                    return i0t %23;
                }
                goto lb48;
                lb48:
                i8t* %25 = i8n 1 as *ptr;
                i8t* %24 = i8s* %8 + i8t* %25;
                i8s* %8 = i8t* %24;
                i8t* %27 = i8n 1 as *ptr;
                i8t* %26 = i8s* %9 + i8t* %27;
                i8s* %9 = i8t* %26;
            }
            goto lb29;
            lb31:
            i64s %28 = alloc;
            i8t %29 = *(i8s* %8);
            i8t %30 = not i8t %29;
            if i8t %30, goto lb76, else goto lb77;
            lb76:
            i8t %31 = *(i8s* %9);
            i8t %32 = not i8t %31;
            i64s %28 = alloc;
            i64t %33 = i8t %32 as i64;
            i64s %28 = i64t %33;
            goto lb78;
            lb77:
            i64s %28 = csu8 0;
            goto lb78;
            lb78:
            i0t %34 = i64s %28;
            return i0t %34;
        }
    }
    fn strlen1(u8* self) -> i32
    {
        u8s* %3 = alloc;
        u8s* %3 = load_arg();
        {
            i32s %4 = alloc;
            i32t %35 = i8n 0 as i32;
            i32s %4 = i32t %35;
            i8s* %5 = alloc;
            u8t* %36 = u8s* %3 + csu8 0;
            i8t* %37 = *(u8t* %36);
            i8s* %5 = i8t* %37;
            lb110:
            i8t %38 = *(i8s* %5);
            if i8t %38, goto lb111, else goto lb112;
            lb111:
            {
                i32t %40 = i8n 1 as i32;
                i32t %39 = i32s %4 + i32t %40;
                i32s %4 = i32t %39;
                i8t* %42 = i8n 1 as *ptr;
                i8t* %41 = i8s* %5 + i8t* %42;
                i8s* %5 = i8t* %41;
            }
            goto lb110;
            lb112:
            return i32s %4;
        }
    }
    fn new0(u8* self, i8* data) -> i0
    {
        u8s* %1 = alloc;
        u8s* %1 = load_arg();
        i8s* %2 = alloc;
        i8s* %2 = load_arg();
        {
            u8t** %43 = u8s* %1 + csu8 0;
            i8t** %44 = u8t** %43;
            *(i8t** %44) = i8s* %2;
        }
    }
    fn _main()
    {
        {
            u8s %10 = arr_alloc(csu8 80);
            u8t* %45 = &(u8s %10);
            use u8t* %45;
            use Hello, World!;
            new0(u8t* %45, Hello, World!);
            u8s %11 = arr_alloc(csu8 80);
            u8t* %46 = &(u8s %11);
            use u8t* %46;
            use Hello, World!;
            new0(u8t* %46, Hello, World!);
            u8t* %47 = &(u8s %10);
            use u8t* %47;
            use u8s %11;
            i0t %48 = equals2(u8t* %47, u8s %11);
            if i0t %48, goto lb186, else goto lb188;
            lb186:
            {
                u8t %49 = i8n 1 as u8;
                exit u8t %49;
            }
            goto lb188;
            lb188:
            u8t %50 = i8n 0 as u8;
            exit u8t %50;
        }
    }
}
/:
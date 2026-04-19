function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }
    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

start(i32 argc, ptr ptr i8 argv) {
    i32 i = 0;
    i32 l = 0;
    i32 r = 0;
    i32 n = strlen(argv[1]);

    while i < n; {
        @[no_fall]
        @[straight]
        switch argv[1][i]; {
            case '['; {
                l += 1;
            }
            case ']'; {
                r += 1;
            }
        }
        i += 1;
    }

    putdigit(l);
    putdigit(r);
    exit 0;
}

:/ OUTPUT
{
    fn strlen0(i8* s) -> i32
    {
        i8s* %0 = alloc(8);
        i8s* %51 = load_arg();
        {
            i32s %1 = alloc(8);
            i32t %11 = i8n 0 as i32;
            i32s %52 = i32t %11;
            future: i32s %54 = previous: i32s %52;
            future: u64s* %53 = previous: u64s* %51;
            i32t %13 = i8n 1 as i32;
            i8t* %15 = i8n 1 as *ptr;
            lb21:
            [u64s %0] u64s* %53 = phi(set [%56, bb6][%51, bb23]);
            [i32s %1] i32s %54 = phi(set [%52, bb23][%55, bb6]);
            i8t %12 = *(i8s* %53);
            if i8t %12, goto lb22, else goto lb23;
            lb22:
            {
                i32t %14 = i32s %54 + i32t %13;
                i32s %55 = i32t %14;
                i8t* %16 = i8s* %53 + i8t* %15;
                i8s* %56 = i8t* %16;
            }
            future: u64s* %53 = previous: u64s* %56;
            future: i32s %54 = previous: i32s %55;
            goto lb21;
            lb23:
            return i32s %54;
        }
    }
    [unused]
    fn putc1(i8 c) -> i0
    {
        i8s %2 = alloc(8);
        i8s %57 = load_arg();
        {
            i8t* %17 = &(i8s %57);
            use i8t* %17;
            syscall, argc: args(i32n 33554436,i8n 1,i8t* %17,i8n 1,);
        }
    }
    fn putdigit2(i32 x) -> i0
    {
        i32s %3 = alloc(8);
        i32s %58 = load_arg();
        {
            i8s %4 = alloc(8);
            i8t %18 = i32s %58 as i8;
            i8s %59 = i8t %18;
            i8t %19 = i8n 48 as i8;
            i8t %20 = i8s %59 + i8t %19;
            i8s %60 = i8t %20;
            use i8s %60;
            call putc1(i8 c) -> i0, argc args(i8s %60,);
        }
    }
    start {
        {
            i32s %5 = alloc(8);
            i32s %61 = load_starg();
            i8s** %6 = alloc(8);
            i8s** %62 = load_starg();
            {
                i32s %7 = alloc(8);
                i32t %21 = i8n 0 as i32;
                i32s %63 = i32t %21;
                i32s %8 = alloc(8);
                i32t %22 = i8n 0 as i32;
                i32s %64 = i32t %22;
                i32s %9 = alloc(8);
                i32t %23 = i8n 0 as i32;
                i32s %65 = i32t %23;
                i32s %10 = alloc(8);
                i8t** %24 = i8s** %62;
                i64t %25 = i8n 1 * cnst? 8;
                i8t** %27 = i64t %25 as *ptr;
                i8t** %26 = i8t** %24 + i8t** %27;
                i8t* %28 = *(i8t** %26);
                use i8t* %28;
                i32t %29 = call strlen0(i8* s) -> i32, argc args(i8t* %28,);
                i32s %66 = i32t %29;
                future: i32s %68 = previous: i32s %65;
                future: i32s %69 = previous: i32s %64;
                future: i32s %67 = previous: i32s %63;
                i32t %32 = i8n 1 as i32;
                i32t %34 = i8n 1 as i32;
                i64t %36 = i8n 1 * cnst? 8;
                i32t %50 = i8n 1 as i32;
                i8t** %38 = i64t %36 as *ptr;
                lb174:
                [i32s %7] i32s %67 = phi(set [%72, bb20][%63, bb24]);
                [i32s %9] i32s %68 = phi(set [%70, bb20][%65, bb24]);
                [i32s %8] i32s %69 = phi(set [%64, bb24][%71, bb20]);
                i32t %30 = i32s %67 < i32s %66;
                if i32t %30, goto lb175, else goto lb176;
                lb175:
                {
                    goto lb186;
                    lb188:
                    {
                        i32t %31 = i32s %69 + i32t %32;
                        i32s %74 = i32t %31;
                    }
                    future: i32s %70 = previous: i32s %68;
                    future: i32s %71 = previous: i32s %74;
                    goto lb187;
                    lb202:
                    {
                        i32t %33 = i32s %68 + i32t %34;
                        i32s %73 = i32t %33;
                    }
                    future: i32s %70 = previous: i32s %73;
                    future: i32s %71 = previous: i32s %69;
                    goto lb187;
                    [unused] goto lb187;
                    lb186:
                    i8t** %35 = i8s** %62;
                    i8t** %37 = i8t** %35 + i8t** %38;
                    i8t* %39 = *(i8t** %37);
                    i8t* %40 = i8t* %39;
                    i64t %41 = i32s %67 * cnst? 1;
                    i8t* %43 = i64t %41 as *ptr;
                    i8t* %42 = i8t* %40 + i8t* %43;
                    i8t %44 = *(i8t* %42);
                    i64t %46 = i8t %44;
                    i8t %45 = i64t %46 == cnst? 91;
                    if i8t %45, goto lb188, else goto lb236;
                    lb236:
                    i64t %48 = i8t %44;
                    i8t %47 = i64t %48 == cnst? 93;
                    if i8t %47, goto lb202, else goto lb240;
                    future: i32s %70 = previous: i32s %68;
                    future: i32s %71 = previous: i32s %69;
                    lb240:
                    lb187:
                    [i32s %9] i32s %70 = phi(set [%68, bb19][%73, bb15][%68, bb14]);
                    [i32s %8] i32s %71 = phi(set [%69, bb19][%69, bb15][%74, bb14]);
                    i32t %49 = i32s %67 + i32t %50;
                    i32s %72 = i32t %49;
                }
                future: i32s %67 = previous: i32s %72;
                future: i32s %68 = previous: i32s %70;
                future: i32s %69 = previous: i32s %71;
                goto lb174;
                lb176:
                use i32s %69;
                call putdigit2(i32 x) -> i0, argc args(i32s %69,);
                use i32s %68;
                call putdigit2(i32 x) -> i0, argc args(i32s %68,);
                exit i8n 0;
            }
        }
    [unused] }
}
/:
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
        i8s* %0 = alloc;
        i8s* %53 = load_arg();
        {
            i32s %1 = alloc;
            i32t %11 = i8n 0 as i32;
            i32s %54 = i32t %11;
            [SSA] future: u64s* %55 <<== previous: u64s* %53;
            [SSA] future: i32s %56 <<== previous: i32s %54;
            i32t %13 = i8n 1 as i32;
            i8t* %15 = i8n 1 as *ptr;
            lb15:
            [base: u64s %0] u64s* %55 = phi(set [%53, bb23][%58, bb6]);
            [base: i32s %1] i32s %56 = phi(set [%54, bb23][%57, bb6]);
            i8t %12 = *(i8s* %55);
            if i8t %12, goto lb16, else goto lb17;
            lb16:
            {
                i32t %14 = i32s %56 + i32t %13;
                i32s %57 = i32t %14;
                i8t* %16 = i8s* %55 + i8t* %15;
                i8s* %58 = i8t* %16;
            }
            [SSA] future: u64s* %55 <<== previous: u64s* %58;
            [SSA] future: i32s %56 <<== previous: i32s %57;
            goto lb15;
            lb17:
            return i32s %56;
        }
    }
    [unused]
    fn putc1(i8 c) -> i0
    {
        i8s %2 = alloc;
        i8s %59 = load_arg();
        {
            i8t* %17 = &(i8s %59);
            use i8t* %17;
            syscall(i32n 33554436, i8n 1, i8t* %17, i8n 1);
        }
    }
    fn putdigit2(i32 x) -> i0
    {
        i32s %3 = alloc;
        i32s %60 = load_arg();
        {
            i8s %4 = alloc;
            i8t %18 = i32s %60 as i8;
            i8s %61 = i8t %18;
            i8t %19 = i8n 48 as i8;
            i8t %20 = i8s %61 + i8t %19;
            i8s %62 = i8t %20;
            use i8s %62;
            putc1(i8s %62);
        }
    }
    start {
        {
            i32s %5 = alloc;
            i32s %63 = load_starg();
            i8s** %6 = alloc;
            i8s** %64 = load_starg();
            {
                i32s %7 = alloc;
                i32t %21 = i8n 0 as i32;
                i32s %65 = i32t %21;
                i32s %8 = alloc;
                i32t %22 = i8n 0 as i32;
                i32s %66 = i32t %22;
                i32s %9 = alloc;
                i32t %23 = i8n 0 as i32;
                i32s %67 = i32t %23;
                i32s %10 = alloc;
                i8t** %24 = i8s** %64;
                i8t %25 = i8n 1 * csu8 8;
                i8t** %27 = i8t %25 as *ptr;
                i8t** %26 = i8t** %24 + i8t** %27;
                i8t* %28 = *(i8t** %26);
                use i8t* %28;
                i32t %29 = strlen0(i8t* %28);
                i32s %68 = i32t %29;
                [SSA] future: i32s %69 <<== previous: i32s %65;
                [SSA] future: i32s %70 <<== previous: i32s %67;
                [SSA] future: i32s %71 <<== previous: i32s %66;
                lb121:
                [base: i32s %7] i32s %69 = phi(set [%74, bb20][%65, bb11]);
                [base: i32s %9] i32s %70 = phi(set [%72, bb20][%67, bb11]);
                [base: i32s %8] i32s %71 = phi(set [%73, bb20][%66, bb11]);
                i32t %30 = i32s %69 < i32s %68;
                if i32t %30, goto lb122, else goto lb123;
                lb122:
                {
                    goto lb129;
                    lb131:
                    {
                        i32t %32 = i8n 1 as i32;
                        i32t %31 = i32s %71 + i32t %32;
                        i32s %76 = i32t %31;
                    }
                    [SSA] future: i32s %72 <<== previous: i32s %70;
                    [SSA] future: i32s %73 <<== previous: i32s %76;
                    goto lb130;
                    lb140:
                    {
                        i32t %34 = i8n 1 as i32;
                        i32t %33 = i32s %70 + i32t %34;
                        i32s %75 = i32t %33;
                    }
                    [SSA] future: i32s %72 <<== previous: i32s %75;
                    [SSA] future: i32s %73 <<== previous: i32s %71;
                    goto lb130;
                    [unused] goto lb130;
                    lb129:
                    i8t** %35 = i8s** %64;
                    i8t %36 = i8n 1 * csu8 8;
                    i8t** %38 = i8t %36 as *ptr;
                    i8t** %37 = i8t** %35 + i8t** %38;
                    i8t* %39 = *(i8t** %37);
                    i8t* %40 = i8t* %39;
                    i32t %41 = i32s %69 * csu8 1;
                    i8t* %43 = i32t %41 as *ptr;
                    i8t* %42 = i8t* %40 + i8t* %43;
                    i8t %44 = *(i8t* %42);
                    i64t %46 = csu8 91 as i64;
                    i64t %47 = i8t %44 as i64;
                    i8t %45 = i64t %47 == i64t %46;
                    if i8t %45, goto lb131, else goto lb165;
                    lb165:
                    i64t %49 = csu8 93 as i64;
                    i64t %50 = i8t %44 as i64;
                    i8t %48 = i64t %50 == i64t %49;
                    if i8t %48, goto lb140, else goto lb170;
                    lb170:
                    [SSA] future: i32s %73 <<== previous: i32s %71;
                    [SSA] future: i32s %72 <<== previous: i32s %70;
                    lb130:
                    [base: i32s %9] i32s %72 = phi(set [%75, bb15][%70, bb19][%70, bb14]);
                    [base: i32s %8] i32s %73 = phi(set [%71, bb19][%76, bb14][%71, bb15]);
                    i32t %52 = i8n 1 as i32;
                    i32t %51 = i32s %69 + i32t %52;
                    i32s %74 = i32t %51;
                }
                [SSA] future: i32s %69 <<== previous: i32s %74;
                [SSA] future: i32s %70 <<== previous: i32s %72;
                [SSA] future: i32s %71 <<== previous: i32s %73;
                goto lb121;
                lb123:
                use i32s %71;
                putdigit2(i32s %71);
                use i32s %70;
                putdigit2(i32s %70);
                exit i8n 0;
            }
        }
    [unused] }
}
/:
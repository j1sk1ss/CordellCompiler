{
    function _strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function _putc(i8 c) => i0 {
        syscall(0x2000004, 1, ref c, 1);
    }

    function _putn(i32 a) => i0 {
        while a > 0; {
            _putc(a % 10 + '0');
            a /= 10;
        }
    }

    function _puts(ptr i8 s) => i0 {
        while dref s; {
            _putc(dref s);
            s += 1;
        }
    }

    function printf(ptr i8 fmt, ...) {
        i32 index = 0;
        i64 fmt_len = _strlen(fmt);
        while index < fmt_len; {
            switch fmt[index]; {
                case '%'; {
                    i8 mode = fmt[index += 1];
                    switch mode; {
                        case 's'; {
                            _puts(poparg as ptr i8);
                            break;
                        }
                        case 'u'; {}
                        case 'i'; {}
                        case 'l'; {}
                        case 'd'; {
                            _putn(poparg as ptr i8);
                            break;
                        }
                    }

                    break;
                }
                default {
                    _putc(fmt[index]);
                    break;
                }
            }

            index += 1;
        }
    }

    start() {
        printf("Hello %s! Im %i yo!\n", "Max", 22);
        exit 0;
    }
}

: OUTPUT
{
    fn _strlen0(i8* s) -> i64
    {
        u64s %0 = alloc(8);
        u64s %0 = load_arg();
        {
            i64s %1 = alloc(8);
            i64s %1 = num? 0;
            lb10:
            u64t %9 = *(u64s %0);
            if u64t %9, goto lb11, else goto lb12;
            lb11:
            {
                u64t %11 = num? 1 as u64;
                u64t %10 = u64s %0 + u64t %11;
                u64s %0 = u64t %10;
                i64t %12 = i64s %1 + num? 1;
                i64s %1 = i64t %12;
            }
            goto lb10;
            lb12:
            return i64s %1;
        }
    }
    fn _putc1(i8 c) -> i0
    {
        i8s %2 = alloc(8);
        i8s %2 = load_arg();
        {
            use num? 33554436;
            use num? 1;
            i8t %13 = &(i8s %2);
            use i8t %13;
            use num? 1;
            syscall, argc: args(num? 33554436,num? 1,i8t %13,num? 1,);
        }
    }
    fn _putn2(i32 a) -> i0
    {
        i32s %3 = alloc(8);
        i32s %3 = load_arg();
        {
            lb53:
            i64t %15 = i32s %3 as i64;
            i64t %14 = i64t %15 > num? 0;
            if i64t %14, goto lb54, else goto lb55;
            lb54:
            {
                i64t %17 = i32s %3 as i64;
                i64t %16 = i64t %17 mod num? 10;
                u64t %19 = i64t %16 as u64;
                u64t %20 = num? 48 as u64;
                u64t %18 = u64t %19 + u64t %20;
                use u64t %18;
                call _putc1(i8 c) -> i0, argc args(u64t %18,);
                i32t %22 = num? 10 as i32;
                i32t %21 = i32s %3 / i32t %22;
                i32s %3 = i32t %21;
            }
            goto lb53;
            lb55:
        }
    }
    fn _puts3(i8* s) -> i0
    {
        u64s %4 = alloc(8);
        u64s %4 = load_arg();
        {
            lb86:
            u64t %23 = *(u64s %4);
            if u64t %23, goto lb87, else goto lb88;
            lb87:
            {
                u64t %24 = *(u64s %4);
                use u64t %24;
                call _putc1(i8 c) -> i0, argc args(u64t %24,);
                u64t %26 = num? 1 as u64;
                u64t %25 = u64s %4 + u64t %26;
                u64s %4 = u64t %25;
            }
            goto lb86;
            lb88:
        }
    }
    fn printf4(i8* fmt, ...)
    {
        u64s %5 = alloc(8);
        u64s %5 = load_arg();
        {
            i32s %6 = alloc(8);
            i32t %27 = num? 0 as i32;
            i32s %6 = i32t %27;
            i64s %7 = alloc(8);
            use u64s %5;
            i64t %28 = call _strlen0(i8* s) -> i64, argc args(u64s %5,);
            i64s %7 = i64t %28;
            lb122:
            i64t %30 = i32s %6 as i64;
            i64t %29 = i64t %30 < i64s %7;
            if i64t %29, goto lb123, else goto lb124;
            lb123:
            {
                goto lb130;
                lb132:
                {
                    i8s %8 = alloc(8);
                    i32t %32 = num? 1 as i32;
                    i32t %31 = i32s %6 + i32t %32;
                    i32s %6 = i32t %31;
                    u64t %33 = u64s %5;
                    i32t %35 = cnst? 1 as i32;
                    i32t %34 = i32s %6 * i32t %35;
                    u64t %37 = i32t %34 as u64;
                    u64t %36 = u64t %33 + u64t %37;
                    i8t %38 = *(u64t %36);
                    i8s %8 = i8t %38;
                    goto lb150;
                    lb152:
                    {
                        u64t %40 = load_arg();
                        u64t %39 = u64t %40 as u64;
                        use u64t %39;
                        call _puts3(i8* s) -> i0, argc args(u64t %39,);
                        goto lb151;
                    }
                    lb161:
                    {
                    }
                    lb164:
                    {
                    }
                    lb167:
                    {
                    }
                    lb170:
                    {
                        u64t %42 = load_arg();
                        u64t %41 = u64t %42 as u64;
                        use u64t %41;
                        call _putn2(i32 a) -> i0, argc args(u64t %41,);
                        goto lb151;
                    }
                    goto lb151;
                    lb150:
                    i8t %44 = cnst? 108 as i8;
                    i8t %43 = i8s %8 < i8t %44;
                    if i8t %43, goto lb180, else goto lb181;
                    lb180:
                    i8t %46 = cnst? 100 as i8;
                    i8t %45 = i8s %8 < i8t %46;
                    if i8t %45, goto lb187, else goto lb188;
                    lb187:
                    goto lb151;
                    lb188:
                    i8t %48 = cnst? 100 as i8;
                    i8t %47 = i8s %8 > i8t %48;
                    if i8t %47, goto lb189, else goto lb190;
                    lb189:
                    i8t %50 = cnst? 105 as i8;
                    i8t %49 = i8s %8 < i8t %50;
                    if i8t %49, goto lb197, else goto lb198;
                    lb197:
                    goto lb151;
                    lb198:
                    i8t %52 = cnst? 105 as i8;
                    i8t %51 = i8s %8 > i8t %52;
                    if i8t %51, goto lb199, else goto lb200;
                    lb199:
                    goto lb151;
                    lb200:
                    goto lb164;
                    lb190:
                    goto lb170;
                    lb181:
                    i8t %54 = cnst? 108 as i8;
                    i8t %53 = i8s %8 > i8t %54;
                    if i8t %53, goto lb182, else goto lb183;
                    lb182:
                    i8t %56 = cnst? 115 as i8;
                    i8t %55 = i8s %8 < i8t %56;
                    if i8t %55, goto lb210, else goto lb211;
                    lb210:
                    goto lb151;
                    lb211:
                    i8t %58 = cnst? 115 as i8;
                    i8t %57 = i8s %8 > i8t %58;
                    if i8t %57, goto lb212, else goto lb213;
                    lb212:
                    i8t %60 = cnst? 117 as i8;
                    i8t %59 = i8s %8 < i8t %60;
                    if i8t %59, goto lb220, else goto lb221;
                    lb220:
                    goto lb151;
                    lb221:
                    i8t %62 = cnst? 117 as i8;
                    i8t %61 = i8s %8 > i8t %62;
                    if i8t %61, goto lb222, else goto lb223;
                    lb222:
                    goto lb151;
                    lb223:
                    goto lb161;
                    lb213:
                    goto lb152;
                    lb183:
                    goto lb167;
                    lb151:
                    goto lb131;
                }
                lb231:
                {
                    u64t %63 = u64s %5;
                    i32t %65 = cnst? 1 as i32;
                    i32t %64 = i32s %6 * i32t %65;
                    u64t %67 = i32t %64 as u64;
                    u64t %66 = u64t %63 + u64t %67;
                    i8t %68 = *(u64t %66);
                    use i8t %68;
                    call _putc1(i8 c) -> i0, argc args(i8t %68,);
                    goto lb131;
                }
                goto lb131;
                lb130:
                u64t %69 = u64s %5;
                i32t %71 = cnst? 1 as i32;
                i32t %70 = i32s %6 * i32t %71;
                u64t %73 = i32t %70 as u64;
                u64t %72 = u64t %69 + u64t %73;
                i8t %74 = *(u64t %72);
                i8t %76 = cnst? 37 as i8;
                i8t %75 = i8t %74 < i8t %76;
                if i8t %75, goto lb255, else goto lb256;
                lb255:
                goto lb231;
                lb256:
                i8t %78 = cnst? 37 as i8;
                i8t %77 = i8t %74 > i8t %78;
                if i8t %77, goto lb257, else goto lb258;
                lb257:
                goto lb231;
                lb258:
                goto lb132;
                lb131:
                i32t %80 = num? 1 as i32;
                i32t %79 = i32s %6 + i32t %80;
                i32s %6 = i32t %79;
            }
            goto lb122;
            lb124:
        }
    }
    start {
        {
            {
                use Hello %s! Im %i yo!
;
                use Max;
                use num? 22;
                call printf4(i8* fmt, ...), argc args(Hello %s! Im %i yo!
,Max,num? 22,);
                exit num? 0;
            }
        }
    }
}
:
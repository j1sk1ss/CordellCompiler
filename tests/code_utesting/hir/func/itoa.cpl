function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    if not num; {
        dref buffer = '0';
        return;
    }

    i32 i = 0;
    arr tmp[32, i8];
    while num > 0 as i32; {
        tmp[i] = '0' + (num % 10) as i8;
        num /= 10;
        i += 1;
    }

    i32 j = 0;
    while i > 0 && j < blen; {
        buffer[j] = tmp[i];
        j += 1;
        i -= 1;
    }
}

glob arr buffer[32, i8] = { 0 };

start() {
    itoa(123321, ref buffer, sizeof(buffer) as i32);
    syscall(0x2000004, 1, ref buffer, sizeof(buffer));
}

:/ OUTPUT
{
    fn itoa0(i32 num, i8* buffer, i32 blen) -> i0
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        i8s* %1 = alloc(8);
        i8s* %1 = load_arg();
        i32s %2 = alloc(8);
        i32s %2 = load_arg();
        {
            i32t %7 = not i32s %0;
            if i32t %7, goto lb16, else goto lb18;
            lb16:
            {
                *(i8s* %1) = i8n 48;
                return ;
            }
            goto lb18;
            lb18:
            i32s %3 = alloc(8);
            i32t %8 = i8n 0 as i32;
            i32s %3 = i32t %8;
            arrs %4 = arr_alloc(i8n 32);
            lb37:
            i32t %9 = i8n 0 as i32;
            i32t %10 = i32s %0 > i32t %9;
            if i32t %10, goto lb38, else goto lb39;
            lb38:
            {
                i32t %13 = i8n 10 as i32;
                i32t %12 = i32s %0 mod i32t %13;
                i8t %11 = i32t %12 as i8;
                i8t %14 = i8n 48 + i8t %11;
                i8t* %15 = &(arrs %4);
                i64t %16 = i32s %3 * cnst? 1;
                i8t* %18 = i64t %16 as *ptr;
                i8t* %17 = i8t* %15 + i8t* %18;
                *(i8t* %17) = i8t %14;
                i32t %20 = i8n 10 as i32;
                i32t %19 = i32s %0 / i32t %20;
                i32s %0 = i32t %19;
                i32t %22 = i8n 1 as i32;
                i32t %21 = i32s %3 + i32t %22;
                i32s %3 = i32t %21;
            }
            goto lb37;
            lb39:
            i32s %5 = alloc(8);
            i32t %23 = i8n 0 as i32;
            i32s %5 = i32t %23;
            lb79:
            i32t %25 = i8n 0 as i32;
            i32t %24 = i32s %3 > i32t %25;
            if i32t %24, goto lb86, else goto lb87;
            lb86:
            i32t %26 = i32s %5 < i32s %2;
            i32t %27 = i32t %26;
            goto lb88;
            lb87:
            i32t %27 = cnst? 0;
            goto lb88;
            lb88:
            if i32t %27, goto lb80, else goto lb81;
            lb80:
            {
                i8t* %28 = &(arrs %4);
                i64t %29 = i32s %3 * cnst? 1;
                i8t* %31 = i64t %29 as *ptr;
                i8t* %30 = i8t* %28 + i8t* %31;
                i8t %32 = *(i8t* %30);
                i8t* %33 = i8s* %1;
                i64t %34 = i32s %5 * cnst? 1;
                i8t* %36 = i64t %34 as *ptr;
                i8t* %35 = i8t* %33 + i8t* %36;
                *(i8t* %35) = i8t %32;
                i32t %38 = i8n 1 as i32;
                i32t %37 = i32s %5 + i32t %38;
                i32s %5 = i32t %37;
                i32t %40 = i8n 1 as i32;
                i32t %39 = i32s %3 - i32t %40;
                i32s %3 = i32t %39;
            }
            goto lb79;
            lb81:
        }
    }
    arrg %6 = arr_alloc(i8n 32);
    start {
        {
            {
                i8t* %41 = &(arrg %6);
                use i8t* %41;
                i32t %42 = cnst? 32 as i32;
                use i32t %42;
                call itoa0(i32 num, i8* buffer, i32 blen) -> i0, argc args(i32n 123321,i8t* %41,i32t %42,);
                i8t* %43 = &(arrg %6);
                use i8t* %43;
                syscall, argc: args(i32n 33554436,i8n 1,i8t* %43,cnst? 32,);
            }
        }
    }
}
/:
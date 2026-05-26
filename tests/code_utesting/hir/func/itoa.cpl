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
        i32s %0 = alloc;
        i32s %0 = load_arg();
        i8s* %1 = alloc;
        i8s* %1 = load_arg();
        i32s %2 = alloc;
        i32s %2 = load_arg();
        {
            i32t %7 = not i32s %0;
            if i32t %7, goto lb19, else goto lb21;
            lb19:
            {
                *(i8s* %1) = i8n 48;
                return ;
            }
            goto lb21;
            lb21:
            i32s %3 = alloc;
            i32t %8 = i8n 0 as i32;
            i32s %3 = i32t %8;
            arrs %4 = arr_alloc(i8n 32);
            lb40:
            i32t %9 = i8n 0 as i32;
            i32t %10 = i32s %0 > i32t %9;
            if i32t %10, goto lb41, else goto lb42;
            lb41:
            {
                i32t %13 = i8n 10 as i32;
                i32t %12 = i32s %0 % i32t %13;
                i8t %11 = i32t %12 as i8;
                i8t %14 = i8n 48 + i8t %11;
                i8t* %15 = &(arrs %4);
                i32t %16 = i32s %3 * csu8 1;
                i8t* %18 = i32t %16 as *ptr;
                i8t* %17 = i8t* %15 + i8t* %18;
                *(i8t* %17) = i8t %14;
                i32t %20 = i8n 10 as i32;
                i32t %19 = i32s %0 / i32t %20;
                i32s %0 = i32t %19;
                i32t %22 = i8n 1 as i32;
                i32t %21 = i32s %3 + i32t %22;
                i32s %3 = i32t %21;
            }
            goto lb40;
            lb42:
            i32s %5 = alloc;
            i32t %23 = i8n 0 as i32;
            i32s %5 = i32t %23;
            lb82:
            i64s %24 = alloc;
            i32t %26 = i8n 0 as i32;
            i32t %25 = i32s %3 > i32t %26;
            if i32t %25, goto lb90, else goto lb91;
            lb90:
            i32t %27 = i32s %5 < i32s %2;
            i64s %24 = alloc;
            i64t %28 = i32t %27 as i64;
            i64s %24 = i64t %28;
            goto lb92;
            lb91:
            i64s %24 = csu8 0;
            goto lb92;
            lb92:
            if i64s %24, goto lb83, else goto lb84;
            lb83:
            {
                i8t* %29 = &(arrs %4);
                i32t %30 = i32s %3 * csu8 1;
                i8t* %32 = i32t %30 as *ptr;
                i8t* %31 = i8t* %29 + i8t* %32;
                i8t %33 = *(i8t* %31);
                i8t* %34 = i8s* %1;
                i32t %35 = i32s %5 * csu8 1;
                i8t* %37 = i32t %35 as *ptr;
                i8t* %36 = i8t* %34 + i8t* %37;
                *(i8t* %36) = i8t %33;
                i32t %39 = i8n 1 as i32;
                i32t %38 = i32s %5 + i32t %39;
                i32s %5 = i32t %38;
                i32t %41 = i8n 1 as i32;
                i32t %40 = i32s %3 - i32t %41;
                i32s %3 = i32t %40;
            }
            goto lb82;
            lb84:
        }
    }
    arrg %6 = arr_alloc(i8n 32);
    fn _main()
    {
        {
            i8t* %42 = &(arrg %6);
            use i8t* %42;
            i32t %43 = csu8 32 as i32;
            use i32t %43;
            itoa0(i32n 123321, i8t* %42, i32t %43);
            i8t* %44 = &(arrg %6);
            use i8t* %44;
            syscall(i32n 33554436, i8n 1, i8t* %44, csu8 32);
        }
    }
}
/:
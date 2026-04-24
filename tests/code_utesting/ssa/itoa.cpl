function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    if not num; {
        dref buffer = '0';
        return;
    }

    i32 i = 0;
    arr tmp[32, i8];
    while num > 0; {
        tmp[i] = '0' + (num % 10) as i8;
        num /= 10;
        i += 1;
    }

    i32 j = 0;
    while i > 0 && j < blen; {
        buffer[j] = tmp[i - 1];
        j += 1;
        i -= 1;
    }

    buffer[j] = 0;
}

glob arr buffer[32, i8] = { 0 };

start() {
    itoa(123321, ref buffer, sizeof(buffer) as i32);
    syscall(0x2000004, 1, ref buffer, 7);
}

:/ OUTPUT
{
    fn itoa0(i32 num, i8* buffer, i32 blen) -> i0
    {
        i32s %0 = alloc(8);
        i32s %50 = load_arg();
        i8s* %1 = alloc(8);
        i8s* %51 = load_arg();
        i32s %2 = alloc(8);
        i32s %52 = load_arg();
        {
            i32t %7 = not i32s %50;
            if i32t %7, goto lb16, else goto lb18;
            lb16:
            {
                *(i8s* %51) = i8n 48;
                return ;
            }
            goto lb18;
            i32t %8 = i8n 0 as i32;
            arrs %54 = arr_alloc(i8n 32);
            i32t %10 = i8n 0 as i32;
            i32t %13 = i8n 10 as i32;
            i8t* %15 = &(arrs %54);
            i32t %20 = i8n 10 as i32;
            i32t %22 = i8n 1 as i32;
            lb18:
            i32s %3 = alloc(8);
            i32s %53 = i32t %8;
            future: i32s %55 = previous: i32s %50;
            future: i32s %56 = previous: i32s %53;
            lb37:
            [i32s %0] i32s %55 = phi(set [%50, bb4][%62, bb6]);
            [i32s %3] i32s %56 = phi(set [%63, bb6][%53, bb4]);
            i32t %9 = i32s %55 > i32t %10;
            if i32t %9, goto lb38, else goto lb39;
            lb38:
            {
                i32t %12 = i32s %55 mod i32t %13;
                i8t %11 = i32t %12 as i8;
                i8t %14 = i8n 48 + i8t %11;
                i32t %16 = i32s %56 * csu8 1;
                i8t* %18 = i32t %16 as *ptr;
                i8t* %17 = i8t* %15 + i8t* %18;
                *(i8t* %17) = i8t %14;
                i32t %19 = i32s %55 / i32t %20;
                i32s %62 = i32t %19;
                i32t %21 = i32s %56 + i32t %22;
                i32s %63 = i32t %21;
            }
            future: i32s %55 = previous: i32s %62;
            future: i32s %56 = previous: i32s %63;
            goto lb37;
            i32t %23 = i8n 0 as i32;
            i32t %25 = i8n 0 as i32;
            i32t %29 = i8n 1 as i32;
            i8t* %30 = &(arrs %54);
            i32t %40 = i8n 1 as i32;
            i32t %42 = i8n 1 as i32;
            lb39:
            i32s %5 = alloc(8);
            i32s %57 = i32t %23;
            future: i32s %58 = previous: i32s %56;
            future: i32s %59 = previous: i32s %57;
            lb79:
            [i32s %3] i32s %58 = phi(set [%56, bb7][%61, bb12]);
            [i32s %5] i32s %59 = phi(set [%60, bb12][%57, bb7]);
            i32t %24 = i32s %58 > i32t %25;
            if i32t %24, goto lb86, else goto lb87;
            lb86:
            i32t %26 = i32s %59 < i32s %52;
            i32t %27 = i32t %26;
            goto lb88;
            lb87:
            i32t %27 = csu8 0;
            goto lb88;
            lb88:
            if i32t %27, goto lb80, else goto lb81;
            lb80:
            {
                i32t %28 = i32s %58 - i32t %29;
                i32t %31 = i32t %28 * csu8 1;
                i8t* %33 = i32t %31 as *ptr;
                i8t* %32 = i8t* %30 + i8t* %33;
                i8t %34 = *(i8t* %32);
                i8t* %35 = i8s* %51;
                i32t %36 = i32s %59 * csu8 1;
                i8t* %38 = i32t %36 as *ptr;
                i8t* %37 = i8t* %35 + i8t* %38;
                *(i8t* %37) = i8t %34;
                i32t %39 = i32s %59 + i32t %40;
                i32s %60 = i32t %39;
                i32t %41 = i32s %58 - i32t %42;
                i32s %61 = i32t %41;
            }
            future: i32s %58 = previous: i32s %61;
            future: i32s %59 = previous: i32s %60;
            goto lb79;
            lb81:
            i8t* %43 = i8s* %51;
            i32t %44 = i32s %59 * csu8 1;
            i8t* %46 = i32t %44 as *ptr;
            i8t* %45 = i8t* %43 + i8t* %46;
            *(i8t* %45) = i8n 0;
        }
    }
    arrg %6 = arr_alloc(i8n 32);
    start {
        {
            {
                i8t* %47 = &(arrg %6);
                use i8t* %47;
                i32t %48 = csu8 32 as i32;
                use i32t %48;
                call itoa0(i32 num, i8* buffer, i32 blen) -> i0, argc args(i32n 123321,i8t* %47,i32t %48,);
                i8t* %49 = &(arrg %6);
                use i8t* %49;
                syscall, argc: args(i32n 33554436,i8n 1,i8t* %49,i8n 7,);
            }
        }
    }
}
/:
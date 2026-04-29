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
        i32s %0 = alloc;
        i32s %51 = load_arg();
        i8s* %1 = alloc;
        i8s* %52 = load_arg();
        i32s %2 = alloc;
        i32s %53 = load_arg();
        {
            i32t %7 = not i32s %51;
            if i32t %7, goto lb16, else goto lb18;
            lb16:
            {
                *(i8s* %52) = i8n 48;
                return ;
            }
            goto lb18;
            lb18:
            i32s %3 = alloc;
            i32t %8 = i8n 0 as i32;
            i32s %54 = i32t %8;
            arrs %55 = arr_alloc(i8n 32);
            [SSA] future: i32s %56 <<== previous: i32s %51;
            [SSA] future: i32s %57 <<== previous: i32s %54;
            i32t %10 = i8n 0 as i32;
            i32t %13 = i8n 10 as i32;
            i8t* %15 = &(arrs %55);
            i32t %20 = i8n 10 as i32;
            i32t %22 = i8n 1 as i32;
            i32t %23 = i8n 0 as i32;
            i32s %58 = i32t %23;
            lb37:
            [base: i32s %0] i32s %56 = phi(set [%67, bb6][%51, bb16]);
            [base: i32s %3] i32s %57 = phi(set [%68, bb6][%54, bb16]);
            i32t %9 = i32s %56 > i32t %10;
            if i32t %9, goto lb38, else goto lb39;
            lb38:
            {
                i32t %12 = i32s %56 % i32t %13;
                i8t %11 = i32t %12 as i8;
                i8t %14 = i8n 48 + i8t %11;
                i32t %16 = i32s %57 * csu8 1;
                i8t* %18 = i32t %16 as *ptr;
                i8t* %17 = i8t* %15 + i8t* %18;
                *(i8t* %17) = i8t %14;
                i32t %19 = i32s %56 / i32t %20;
                i32s %67 = i32t %19;
                i32t %21 = i32s %57 + i32t %22;
                i32s %68 = i32t %21;
            }
            [SSA] future: i32s %56 <<== previous: i32s %67;
            [SSA] future: i32s %57 <<== previous: i32s %68;
            goto lb37;
            lb39:
            i32s %5 = alloc;
            [SSA] future: i32s %59 <<== previous: i32s %57;
            [SSA] future: i32s %60 <<== previous: i32s %58;
            i32t %26 = i8n 0 as i32;
            i64s %61 = csu8 0;
            i32t %30 = i8n 1 as i32;
            i8t* %31 = &(arrs %55);
            i8t* %36 = i8s* %52;
            i32t %41 = i8n 1 as i32;
            i32t %43 = i8n 1 as i32;
            i8t* %44 = i8s* %52;
            lb79:
            [base: i32s %3] i32s %59 = phi(set [%57, bb15][%64, bb12]);
            [base: i64s %24] i64s %65 = phi(set [%62, bb12]);
            [base: i32s %5] i32s %60 = phi(set [%63, bb12][%58, bb15]);
            i64s %62 = alloc;
            i32t %25 = i32s %59 > i32t %26;
            if i32t %25, goto lb87, else goto lb88;
            lb87:
            i32t %27 = i32s %60 < i32s %53;
            i64s %62 = alloc;
            i64t %28 = i32t %27 as i64;
            i64s %66 = i64t %28;
            [SSA] future: i64s %62 <<== previous: i64s %66;
            goto lb89;
            lb88:
            [SSA] future: i64s %62 <<== previous: i64s %61;
            goto lb89;
            lb89:
            [base: i64s %24] i64s %62 = phi(set [%66, bb9][%61, bb10]);
            if i64s %62, goto lb80, else goto lb81;
            lb80:
            {
                i32t %29 = i32s %59 - i32t %30;
                i32t %32 = i32t %29 * csu8 1;
                i8t* %34 = i32t %32 as *ptr;
                i8t* %33 = i8t* %31 + i8t* %34;
                i8t %35 = *(i8t* %33);
                i32t %37 = i32s %60 * csu8 1;
                i8t* %39 = i32t %37 as *ptr;
                i8t* %38 = i8t* %36 + i8t* %39;
                *(i8t* %38) = i8t %35;
                i32t %40 = i32s %60 + i32t %41;
                i32s %63 = i32t %40;
                i32t %42 = i32s %59 - i32t %43;
                i32s %64 = i32t %42;
            }
            [SSA] future: i32s %59 <<== previous: i32s %64;
            [SSA] future: i64s %65 <<== previous: i64s %62;
            [SSA] future: i32s %60 <<== previous: i32s %63;
            goto lb79;
            lb81:
            i32t %45 = i32s %60 * csu8 1;
            i8t* %47 = i32t %45 as *ptr;
            i8t* %46 = i8t* %44 + i8t* %47;
            *(i8t* %46) = i8n 0;
        }
    }
    arrg %6 = arr_alloc(i8n 32);
    start {
        {
            {
                i8t* %48 = &(arrg %6);
                use i8t* %48;
                i32t %49 = csu8 32 as i32;
                use i32t %49;
                itoa0(i32n 123321, i8t* %48, i32t %49);
                i8t* %50 = &(arrg %6);
                use i8t* %50;
                syscall(i32n 33554436, i8n 1, i8t* %50, i8n 7);
            }
        }
    }
}
/:
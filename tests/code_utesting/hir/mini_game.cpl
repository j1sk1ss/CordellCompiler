function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

function strcmp(ptr i8 a, ptr i8 b) -> i8 {
    while dref a && dref b; {
        if dref a != dref b; return 0;
        a += 1;
        b += 1;
    }
    
    return dref a == dref b;
}

function print(ptr i8 msg) -> i0 {
    syscall(0x2000004, 1, msg, strlen(msg));
}

start(i32 a, ptr ptr i8 answers) {
    print(ref "Who is better? (max/chloe)\n");
    print(ref "You have written: ");
    print(answers[1]);
    print(ref "\n");
    if not strcmp(answers[1], "max"); print(ref "Wrong!\n");
    else print(ref "Correct!\n");
}

:/ OUTPUT
{
    fn strlen0(i8* s) -> i32
    {
        i8s* %0 = alloc;
        i8s* %0 = load_arg();
        {
            i32s %1 = alloc;
            i32t %7 = i8n 0 as i32;
            i32s %1 = i32t %7;
            lb16:
            i8t %8 = *(i8s* %0);
            if i8t %8, goto lb17, else goto lb18;
            lb17:
            {
                i32t %9 = i8n 1 as i32;
                i32t %10 = i32s %1 + i32t %9;
                i32s %1 = i32t %10;
                i8t* %11 = i8n 1 as *ptr;
                i8t* %12 = i8s* %0 + i8t* %11;
                i8s* %0 = i8t* %12;
            }
            goto lb16;
            lb18:
            return i32s %1;
        }
    }
    fn strcmp1(i8* a, i8* b) -> i8
    {
        i8s* %2 = alloc;
        i8s* %2 = load_arg();
        i8s* %3 = alloc;
        i8s* %3 = load_arg();
        {
            lb52:
            i64s %13 = alloc;
            i8t %14 = *(i8s* %2);
            if i8t %14, goto lb58, else goto lb59;
            lb58:
            i8t %15 = *(i8s* %3);
            i64s %13 = alloc;
            i64t %16 = i8t %15 as i64;
            i64s %13 = i64t %16;
            goto lb60;
            lb59:
            i64s %13 = csu8 0;
            goto lb60;
            lb60:
            if i64s %13, goto lb53, else goto lb54;
            lb53:
            {
                i8t %17 = *(i8s* %2);
                i8t %18 = *(i8s* %3);
                i8t %19 = i8t %17 != i8t %18;
                if i8t %19, goto lb69, else goto lb71;
                lb69:
                {
                    return i8n 0;
                }
                goto lb71;
                lb71:
                i8t* %21 = i8n 1 as *ptr;
                i8t* %20 = i8s* %2 + i8t* %21;
                i8s* %2 = i8t* %20;
                i8t* %23 = i8n 1 as *ptr;
                i8t* %22 = i8s* %3 + i8t* %23;
                i8s* %3 = i8t* %22;
            }
            goto lb52;
            lb54:
            i8t %24 = *(i8s* %2);
            i8t %25 = *(i8s* %3);
            i8t %26 = i8t %24 == i8t %25;
            return i8t %26;
        }
    }
    fn print2(i8* msg) -> i0
    {
        i8s* %4 = alloc;
        i8s* %4 = load_arg();
        {
            use i8s* %4;
            use i8s* %4;
            i32t %27 = strlen0(i8s* %4);
            use i32t %27;
            syscall(i32n 33554436, i8n 1, i8s* %4, i32t %27);
        }
    }
    fn _main(i32 a, i8** answers)
    {
        i32s %5 = alloc;
        i32s %5 = load_starg();
        i8s** %6 = alloc;
        i8s** %6 = load_starg();
        {
            i8t* %28 = &(Who is better? (max/chloe)
);
            use i8t* %28;
            print2(i8t* %28);
            i8t* %29 = &(You have written: );
            use i8t* %29;
            print2(i8t* %29);
            i8t** %30 = i8s** %6;
            i8t %31 = i8n 1 * csu8 8;
            i8t** %33 = i8t %31 as *ptr;
            i8t** %32 = i8t** %30 + i8t** %33;
            i8t* %34 = *(i8t** %32);
            use i8t* %34;
            print2(i8t* %34);
            i8t* %35 = &(
);
            use i8t* %35;
            print2(i8t* %35);
            i8t** %36 = i8s** %6;
            i8t %37 = i8n 1 * csu8 8;
            i8t** %39 = i8t %37 as *ptr;
            i8t** %38 = i8t** %36 + i8t** %39;
            i8t* %40 = *(i8t** %38);
            use i8t* %40;
            use max;
            i8t %41 = strcmp1(i8t* %40, max);
            i8t %42 = not i8t %41;
            if i8t %42, goto lb164, else goto lb165;
            lb164:
            {
                i8t* %43 = &(Wrong!
);
                use i8t* %43;
                print2(i8t* %43);
            }
            goto lb166;
            lb165:
            {
                i8t* %44 = &(Correct!
);
                use i8t* %44;
                print2(i8t* %44);
            }
            goto lb166;
            lb166:
        }
    }
}
/:
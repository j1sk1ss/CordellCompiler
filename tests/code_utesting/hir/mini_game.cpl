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
        i8s* %0 = alloc(8);
        i8s* %0 = load_arg();
        {
            i32s %1 = alloc(8);
            i32t %7 = i8n 0 as i32;
            i32s %1 = i32t %7;
            lb15:
            i8t %8 = *(i8s* %0);
            if i8t %8, goto lb16, else goto lb17;
            lb16:
            {
                i32t %9 = i8n 1 as i32;
                i32t %10 = i32s %1 + i32t %9;
                i32s %1 = i32t %10;
                i8t* %11 = i8n 1 as *ptr;
                i8t* %12 = i8s* %0 + i8t* %11;
                i8s* %0 = i8t* %12;
            }
            goto lb15;
            lb17:
            return i32s %1;
        }
    }
    fn strcmp1(i8* a, i8* b) -> i8
    {
        i8s* %2 = alloc(8);
        i8s* %2 = load_arg();
        i8s* %3 = alloc(8);
        i8s* %3 = load_arg();
        {
            lb49:
            i8t %13 = *(i8s* %2);
            if i8t %13, goto lb54, else goto lb55;
            lb54:
            i8t %14 = *(i8s* %3);
            i8t %15 = i8t %14;
            goto lb56;
            lb55:
            i8t %15 = cnst? 0;
            goto lb56;
            lb56:
            if i8t %15, goto lb50, else goto lb51;
            lb50:
            {
                i8t %16 = *(i8s* %2);
                i8t %17 = *(i8s* %3);
                i8t %18 = i8t %16 != i8t %17;
                if i8t %18, goto lb63, else goto lb65;
                lb63:
                {
                    return i8n 0;
                }
                goto lb65;
                lb65:
                i8t* %20 = i8n 1 as *ptr;
                i8t* %19 = i8s* %2 + i8t* %20;
                i8s* %2 = i8t* %19;
                i8t* %22 = i8n 1 as *ptr;
                i8t* %21 = i8s* %3 + i8t* %22;
                i8s* %3 = i8t* %21;
            }
            goto lb49;
            lb51:
            i8t %23 = *(i8s* %2);
            i8t %24 = *(i8s* %3);
            i8t %25 = i8t %23 == i8t %24;
            return i8t %25;
        }
    }
    fn print2(i8* msg) -> i0
    {
        i8s* %4 = alloc(8);
        i8s* %4 = load_arg();
        {
            use i8s* %4;
            use i8s* %4;
            i32t %26 = call strlen0(i8* s) -> i32, argc args(i8s* %4,);
            use i32t %26;
            syscall, argc: args(i32n 33554436,i8n 1,i8s* %4,i32t %26,);
        }
    }
    start {
        {
            i32s* %5 = alloc(8);
            i32s* %5 = load_starg();
            i8s** %6 = alloc(8);
            i8s** %6 = load_starg();
            {
                i64t* %27 = &(Who is better? (max/chloe)
);
                use i64t* %27;
                call print2(i8* msg) -> i0, argc args(i64t* %27,);
                i64t* %28 = &(You have written: );
                use i64t* %28;
                call print2(i8* msg) -> i0, argc args(i64t* %28,);
                i8t** %29 = i8s** %6;
                i64t %30 = i8n 1 * cnst? 8;
                i8t** %32 = i64t %30 as *ptr;
                i8t** %31 = i8t** %29 + i8t** %32;
                i8t* %33 = *(i8t** %31);
                use i8t* %33;
                call print2(i8* msg) -> i0, argc args(i8t* %33,);
                i64t* %34 = &(
);
                use i64t* %34;
                call print2(i8* msg) -> i0, argc args(i64t* %34,);
                i8t** %35 = i8s** %6;
                i64t %36 = i8n 1 * cnst? 8;
                i8t** %38 = i64t %36 as *ptr;
                i8t** %37 = i8t** %35 + i8t** %38;
                i8t* %39 = *(i8t** %37);
                use i8t* %39;
                use max;
                i8t %40 = call strcmp1(i8* a, i8* b) -> i8, argc args(i8t* %39,max,);
                i8t %41 = not i8t %40;
                if i8t %41, goto lb157, else goto lb158;
                lb157:
                {
                    i64t* %42 = &(Wrong!
);
                    use i64t* %42;
                    call print2(i8* msg) -> i0, argc args(i64t* %42,);
                }
                goto lb159;
                lb158:
                {
                    i64t* %43 = &(Correct!
);
                    use i64t* %43;
                    call print2(i8* msg) -> i0, argc args(i64t* %43,);
                }
                goto lb159;
                lb159:
            }
        }
    }
}
/:
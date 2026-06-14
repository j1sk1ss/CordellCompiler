container node {
    ptr node next;
    i32 value;

    @[self]
    function total2(ptr node self) -> i32 {
        return self.value + self.next.value;
    }
}

container graph {
    node head;
    node tail;

    @[self]
    function score(ptr graph self) -> i32 {
        return self.head.total2();
    }
}

start() {
    graph g;
    g.head.value = 18 as i32;
    g.tail.value = 24 as i32;
    g.head.next = ref g.tail;

    exit g.score() as u8;
}

:/ OUTPUT
{
    fn total20(u8* self) -> i32
    {
        u8s* %2 = alloc;
        u8s* %2 = load_arg();
        {
            u8t* %7 = u8s* %2 + csu8 8;
            i32t %8 = *(u8t* %7);
            u8t* %9 = u8s* %2 + csu8 0;
            u8t** %11 = u8t* %9 as *ptr;
            u8t* %10 = *(u8t** %11);
            u8t* %12 = u8t* %10 + csu8 8;
            i32t %13 = *(u8t* %12);
            i32t %14 = i32t %8 + i32t %13;
            return i32t %14;
        }
    }
    fn score1(u8* self) -> i32
    {
        u8s* %5 = alloc;
        u8s* %5 = load_arg();
        {
            u8t* %15 = u8s* %5 + csu8 0;
            use u8t* %15;
            i32t %16 = total20(u8t* %15);
            return i32t %16;
        }
    }
    fn _main()
    {
        {
            u8s %6 = arr_alloc(csu8 32);
            i32t %17 = i8n 18 as i32;
            u8t* %18 = &(u8s %6);
            u8t* %19 = u8t* %18 + csu8 0;
            u8t* %20 = u8t* %19 + csu8 8;
            i32t* %21 = u8t* %20 as *ptr;
            *(i32t* %21) = i32t %17;
            i32t %22 = i8n 24 as i32;
            u8t* %23 = &(u8s %6);
            u8t* %24 = u8t* %23 + csu8 16;
            u8t* %25 = u8t* %24 + csu8 8;
            i32t* %26 = u8t* %25 as *ptr;
            *(i32t* %26) = i32t %22;
            u8t* %27 = &(u8s %6);
            u8t* %28 = u8t* %27 + csu8 16;
            u8t* %29 = &(u8s %6);
            u8t* %30 = u8t* %29 + csu8 0;
            u8t** %31 = u8t* %30 + csu8 0;
            u8t** %32 = u8t** %31 as *ptr;
            *(u8t** %32) = u8t* %28;
            u8t* %34 = &(u8s %6);
            use u8t* %34;
            i32t %35 = score1(u8t* %34);
            u8t %33 = i32t %35 as u8;
            exit u8t %33;
        }
    }
}
/:
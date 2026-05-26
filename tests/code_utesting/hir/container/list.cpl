container node {
    ptr node next;
    ptr node prev;
}

start() {
    node nd;
    node next;
    node prev;

    nd.next = ref next;
    nd.prev = ref prev;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %2 = arr_alloc(csu8 16);
            u8s %3 = arr_alloc(csu8 16);
            u8s %4 = arr_alloc(csu8 16);
            u8t* %5 = &(u8s %3);
            u8t* %6 = &(u8s %2);
            u8t** %7 = u8t* %6 + csu8 0;
            u8t** %8 = u8t** %7;
            *(u8t** %8) = u8t* %5;
            u8t* %9 = &(u8s %4);
            u8t* %10 = &(u8s %2);
            u8t** %11 = u8t* %10 + csu8 16;
            u8t** %12 = u8t** %11;
            *(u8t** %12) = u8t* %9;
        }
    }
}
/:
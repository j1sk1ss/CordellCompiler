@[align(1)]
container node {
    i32     a;
    ptr i32 b;
}

function foo(ptr node nd) {
    return nd.a + nd.b;
}

:/ OUTPUT
{
    fn foo0(u8* nd)
    {
        u8s* %2 = alloc;
        u8s* %2 = load_arg();
        {
            u8t* %3 = u8s* %2 + csu8 0;
            i32t %4 = *(u8t* %3);
            u8t* %5 = u8s* %2 + csu8 4;
            i32t* %6 = *(u8t* %5);
            i32t* %8 = i32t %4 as *ptr;
            i32t* %7 = i32t* %8 + i32t* %6;
            return i32t* %7;
        }
    }
}
/:
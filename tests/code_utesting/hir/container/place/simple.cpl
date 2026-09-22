interface a {
   @[self] function foo(ptr a self) -> i0;
}
container b::a {
   @[override] function foo(ptr b self) -> i0 {
   }
}

start() {
    ptr i0 addr = 0x0;
    ptr b instance = place(addr, b);
}

:/ OUTPUT
{
    fn foo__b1(u8* self) -> i0
    {
        u8s* %1 = alloc;
        u8s* %1 = load_arg();
        {
        }
    }
    fn _main()
    {
        {
            i0s* %2 = alloc;
            i0t* %4 = i8n 0 as *ptr;
            i0s* %2 = i0t* %4;
            u8s* %3 = alloc;
            i0t* %5 = &(vtable_0);
            *(i0s* %2) = i0t* %5;
            u8t* %6 = i0s* %2 as *ptr;
            u8s* %3 = u8t* %6;
        }
    }
}
/:
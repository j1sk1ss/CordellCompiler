container smth {
}

function foo(ptr smth s);
function foo(f64 s);

start() {
    smth s;
    foo(ref s);
    foo(1 as f64);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %2 = arr_alloc(csu8 0);
            u8t* %3 = &(u8s %2);
            use u8t* %3;
            foo0(u8t* %3);
            f64t %4 = i8n 1 as f64;
            use f64t %4;
            foo1(f64t %4);
        }
    }
}
/:
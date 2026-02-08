{
    function foo(str msg) {
        msg[0] = 'A';
    }

    start() {
        foo("Hello!");
    }
}

: OUTPUT
{
    fn foo0(str msg)
    {
        strs* %0 = alloc(8);
        strs* %0 = load_arg();
        {
            u64t %1 = &(strs* %0);
            i64t %2 = num? 0 * cnst? 1;
            u64t %4 = i64t %2 as u64;
            u64t %3 = u64t %1 + u64t %4;
            i8t %5 = num? 65 as i8;
            *(u64t %3) = i8t %5;
        }
    }
    
    start {
        {
            {
                use Hello!;
                call foo0(str msg), argc args(Hello!,);
            }
        }
    }
}
:
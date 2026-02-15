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
        strs %0 = alloc(8);
        strs %0 = load_arg();
        {
            i8t* %1 = &(strs %0);
            i64t %2 = num? 0 * cnst? 1;
            i8t* %4 = i64t %2 as *ptr;
            i8t* %3 = i8t* %1 + i8t* %4;
            i8t %5 = num? 65 as i8;
            *(i8t* %3) = i8t %5;
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
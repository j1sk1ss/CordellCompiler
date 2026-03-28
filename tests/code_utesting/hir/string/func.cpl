function foo(str msg) {
    msg[0] = 'A' as i8;
}

start() {
    foo("Hello!");
}

:/ OUTPUT
{
    fn foo0(str msg)
    {
        strs %0 = alloc(8);
        strs %0 = load_arg();
        {
            i8t %1 = num? 65 as i8;
            i8t* %2 = &(strs %0);
            i64t %3 = num? 0 * cnst? 1;
            i8t* %5 = i64t %3 as *ptr;
            i8t* %4 = i8t* %2 + i8t* %5;
            *(i8t* %4) = i8t %1;
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
/:

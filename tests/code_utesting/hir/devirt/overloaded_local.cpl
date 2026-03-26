start() {
    function local(i32 a) {
        return 1;
    }
    function local(i16 a) {
        return 2;
    }
    function local(i8 a) {
        return 3;
    }
    exit local(1 as i32) + local(1 as i16) + local(1 as i8);
}

: OUTPUT
{
    start {
        {
            {
                goto lb{X}
                fn local0(i32 a)
                {
                    i32s %0 = alloc(8);
                    i32s %0 = load_arg();
                    {
                        return num? 1;
                    }
                }
                lb{X}
                goto lb{X}
                fn local1(i16 a)
                {
                    i16s %1 = alloc(8);
                    i16s %1 = load_arg();
                    {
                        return num? 2;
                    }
                }
                lb{X}
                goto lb{X}
                fn local2(i8 a)
                {
                    i8s %2 = alloc(8);
                    i8s %2 = load_arg();
                    {
                        return num? 3;
                    }
                }
                lb{X}
                i32t %3 = num? 1 as i32;
                use i32t %3;
                i64t %4 = call local0(i32 a), argc args(i32t %3,);
                i16t %5 = num? 1 as i16;
                use i16t %5;
                i64t %6 = call local1(i16 a), argc args(i16t %5,);
                i64t %7 = i64t %4 + i64t %6;
                i8t %8 = num? 1 as i8;
                use i8t %8;
                i64t %9 = call local2(i8 a), argc args(i8t %8,);
                i64t %10 = i64t %7 + i64t %9;
                exit i64t %10;
            }
        }
    }
}
:
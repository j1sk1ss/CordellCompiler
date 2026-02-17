{
    function chloe(i32 a, i32 b = 10) -> i0;
    function chloe(i64 a, i32 b = 10) -> i0;
    start() {
        chloe(10 as i32);
        chloe(10 as i64);
        exit 0;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32t %4 = num? 10 as i32;
                use i32t %4;
                use num? 10;
                call chloe0(i32 a, i32 b) -> i0, argc args(i32t %4,num? 10,);
                i64t %5 = num? 10 as i64;
                use i64t %5;
                use num? 10;
                call chloe1(i64 a, i32 b) -> i0, argc args(i64t %5,num? 10,);
                exit num? 0;
            }
        }
    }
}
:
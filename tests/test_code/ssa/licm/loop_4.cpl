{
    function foo(i32 a);
    start() {
        loop foo(10 + 10 * 1);
    }
}

: OUTPUT
{
    start {
        {
            {
                i64t %1 = num? 10 * num? 1;
                u64t %3 = num? 10 as u64;
                u64t %4 = i64t %1 as u64;
                u64t %2 = u64t %3 + u64t %4;
                use u64t %2;
                lb4:
                goto lb5;
                lb5:
                {
                    call foo0(i32 a), argc args(u64t %2,);
                }
                goto lb4;
                lb6:
            }
        }
    }
}
:
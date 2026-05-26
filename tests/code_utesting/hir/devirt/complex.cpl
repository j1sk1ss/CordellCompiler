function chloe(i32 a, i32 b = 10) -> i0;
function chloe(i64 a, i32 b = 10) -> i0;
start() {
    chloe(10 as i32);
    chloe(10 as i64);
    exit 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %4 = i8n 10 as i32;
            use i32t %4;
            chloe0(i32t %4, i8n 10);
            i64t %5 = i8n 10 as i64;
            use i64t %5;
            chloe1(i64t %5, i8n 10);
            u8t %6 = i8n 0 as u8;
            exit u8t %6;
        }
    }
}
/:
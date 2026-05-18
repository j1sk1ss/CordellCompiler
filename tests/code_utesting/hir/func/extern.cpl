extern function printf(ptr i8 fmt, ...) -> i32;
extern ptr i8 BUFFER;
start() {
    printf(BUFFER, 10);
}

:/ OUTPUT
{
    (fun) extern(csu8 0);
    (var) extern(csu8 1);
    fn _main()
    {
        {
            use i8g* %1;
            (printf0i8g* %1, i8n 10);
        }
    }
}
/:
container std {
    function printf();
}

function std::printf() {
    return 1;
}

start() {
    std::printf();
    exit 0;
}

:/ OUTPUT
{
    fn printf0__std()
    {
        {
            return i8n 1;
        }
    }
    fn _main()
    {
        {
            printf0__std();
            u8t %0 = i8n 0 as u8;
            exit u8t %0;
        }
    }
}
/:
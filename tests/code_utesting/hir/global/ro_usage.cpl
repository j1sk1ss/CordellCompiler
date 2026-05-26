ro i32 flag = 1 as i32;
start() {
    if flag; {
        exit 1;
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            if i32g %0, goto lb8, else goto lb10;
            lb8:
            {
                u8t %1 = i8n 1 as u8;
                exit u8t %1;
            }
            goto lb10;
            lb10:
        }
    }
}
/:
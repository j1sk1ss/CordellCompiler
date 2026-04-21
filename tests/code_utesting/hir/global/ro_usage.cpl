ro i32 flag = 1 as i32;
start() {
    if flag; {
        exit 1;
    }
}

:/ OUTPUT
{
    start {
        {
            {
                if i32g %0, goto lb8, else goto lb10;
                lb8:
                {
                    exit i8n 1;
                }
                goto lb10;
                lb10:
            }
        }
    }
}
/:
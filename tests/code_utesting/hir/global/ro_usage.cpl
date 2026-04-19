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
                if i32g %0, goto lb11, else goto lb13;
                lb11:
                {
                    exit i8n 1;
                }
                goto lb13;
                lb13:
            }
        }
    }
}
/:
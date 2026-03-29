ro i32 flag = 1 as i32;
start() {
    if flag; {
        exit 1;
    }
}

:/ OUTPUT
{
    i32g %0 = alloc(8);
    i32t %1 = i8n 1 as i32;
    i32g %0 = i32t %1;
    start {
        {
            {
                if i32g %0, goto lb{X} else goto lb{X}
                lb{X}
                {
                    exit i8n 1;
                }
                goto lb{X}
                lb{X}
            }
        }
    }
}
/:

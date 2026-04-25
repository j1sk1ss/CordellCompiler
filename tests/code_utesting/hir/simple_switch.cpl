start() {
    i32 tag;
    @[no_fall]
    @[straight]
    switch tag; {
        case 1; { exit 1; }
        default { exit 0; }
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                goto lb9;
                lb11:
                {
                    exit i8n 1;
                }
                goto lb10;
                lb16:
                {
                    exit i8n 0;
                }
                goto lb10;
                goto lb10;
                lb9:
                i64t %2 = csu8 1 as i64;
                i64t %3 = i32s %0 as i64;
                i8t %1 = i64t %3 == i64t %2;
                if i8t %1, goto lb11, else goto lb23;
                lb23:
                goto lb16;
                lb10:
            }
        }
    }
}
/:
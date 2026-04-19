start(i32 a) {
    if a == 1; {
        exit 1;
    }
    else if a == 2; {
        exit 2;
    }
    else {
        exit 3;
    }

    exit 4;
}

:/ OUTPUT
setpos, line=2, column=86, file={X}
{
setpos, line=1, column=7, file={X}
setpos, line=1, column=7, file={X}
    start {
        {
            i32s %0 = alloc(8);
            i32s %0 = load_starg();
setpos, line=2, column=8, file={X}
            {
setpos, line=2, column=8, file={X}
setpos, line=2, column=8, file={X}
setpos, line=2, column=15, file={X}
setpos, line=2, column=11, file={X}
setpos, line=2, column=18, file={X}
                i32t %2 = i8n 1 as i32;
                i32t %1 = i32s %0 == i32t %2;
                if i32t %1, goto lb13, else goto lb14;
                lb13:
                {
setpos, line=3, column=14, file={X}
setpos, line=3, column=14, file={X}
setpos, line=3, column=17, file={X}
                    exit i8n 1;
                }
                goto lb15;
                lb14:
                {
setpos, line=5, column=14, file={X}
setpos, line=5, column=14, file={X}
setpos, line=5, column=21, file={X}
setpos, line=5, column=17, file={X}
setpos, line=5, column=24, file={X}
                    i32t %4 = i8n 2 as i32;
                    i32t %3 = i32s %0 == i32t %4;
                    if i32t %3, goto lb32, else goto lb33;
                    lb32:
                    {
setpos, line=6, column=14, file={X}
setpos, line=6, column=14, file={X}
setpos, line=6, column=17, file={X}
                        exit i8n 2;
                    }
                    goto lb34;
                    lb33:
                    {
setpos, line=9, column=14, file={X}
setpos, line=9, column=14, file={X}
setpos, line=9, column=17, file={X}
                        exit i8n 3;
                    }
                    goto lb34;
                    lb34:
                }
                goto lb15;
                lb15:
setpos, line=12, column=10, file={X}
setpos, line=12, column=13, file={X}
                exit i8n 4;
            }
        }
    }
}
/:
start() {
    @[cold] if 1; {
        exit 1;
    }
    else {
        exit 2;
    }

    @[cold] if 2; {
        exit 1;
    }
    else {
        exit 2;
    }

    @[cold] if 3; {
        exit 1;
    }
    else {
        exit 2;
    }
}

:/ OUTPUT
{
    start {
        {
            {
                if i8n 1, goto lb7, else goto lb8;
                lb8:
                {
                    exit i8n 2;
                }
                goto lb9;
                lb9:
                if i8n 2, goto lb20, else goto lb21;
                lb21:
                {
                    exit i8n 2;
                }
                goto lb22;
                lb22:
                if i8n 3, goto lb33, else goto lb34;
                lb34:
                {
                    exit i8n 2;
                }
                goto lb35;
                lb35:
            }
            exit csu8 0;
            lb7:
            {
                exit i8n 1;
            }
            goto lb9;
            lb20:
            {
                exit i8n 1;
            }
            goto lb22;
            lb33:
            {
                exit i8n 1;
            }
            goto lb35;
        }
    }
}
/:
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
                if i8n 1, goto lb10, else goto lb11;
                lb11:
                {
                    exit i8n 2;
                }
                goto lb12;
                lb12:
                if i8n 2, goto lb28, else goto lb29;
                lb29:
                {
                    exit i8n 2;
                }
                goto lb30;
                lb30:
                if i8n 3, goto lb46, else goto lb47;
                lb47:
                {
                    exit i8n 2;
                }
                goto lb48;
                lb48:
            }
            exit cnst? 0;
            lb10:
            {
                exit i8n 1;
            }
            goto lb12;
            lb28:
            {
                exit i8n 1;
            }
            goto lb30;
            lb46:
            {
                exit i8n 1;
            }
            goto lb48;
        }
    }
}
/:
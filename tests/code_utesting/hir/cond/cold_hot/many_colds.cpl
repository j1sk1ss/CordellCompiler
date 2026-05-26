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
    fn _main()
    {
        {
            if i8n 1, goto lb7, else goto lb8;
            lb8:
            {
                u8t %1 = i8n 2 as u8;
                exit u8t %1;
            }
            goto lb9;
            lb9:
            if i8n 2, goto lb22, else goto lb23;
            lb23:
            {
                u8t %3 = i8n 2 as u8;
                exit u8t %3;
            }
            goto lb24;
            lb24:
            if i8n 3, goto lb37, else goto lb38;
            lb38:
            {
                u8t %5 = i8n 2 as u8;
                exit u8t %5;
            }
            goto lb39;
            lb39:
        }
        exit csu8 0;
        lb7:
        {
            u8t %0 = i8n 1 as u8;
            exit u8t %0;
        }
        goto lb9;
        lb22:
        {
            u8t %2 = i8n 1 as u8;
            exit u8t %2;
        }
        goto lb24;
        lb37:
        {
            u8t %4 = i8n 1 as u8;
            exit u8t %4;
        }
        goto lb39;
    }
}
/:
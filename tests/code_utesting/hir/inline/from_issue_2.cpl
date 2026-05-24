: ONLY_THIS :
function dealloc(ptr i0 mem) -> i0 {
    if not mem; {
        return;
    }
    dref mem = 1;
}

start() {
    dealloc(0);
}

:/ OUTPUT
{
    fn dealloc0(i0* mem) -> i0
    {
        i0s* %0 = alloc;
        i0s* %0 = load_arg();
        {
            i0t %1 = not i0s* %0;
            if i0t %1, goto lb11, else goto lb13;
            lb11:
            {
                return ;
            }
            goto lb13;
            lb13:
            i0t %2 = i8n 1;
            *(i0s* %0) = i0t %2;
        }
    }
    fn _main()
    {
        {
            i0s* %0 = i8n 0;
            i0t %1 = not i0s* %0;
            if i0t %1, goto lb55, else goto lb56;
            lb55:
            {
                goto lb40;
            }
            goto lb56;
            lb56:
            i0t %2 = i8n 1;
            *(i0s* %0) = i0t %2;
            lb40:
            [unused] dealloc0(i8n 0);
        }
    }
}
/:
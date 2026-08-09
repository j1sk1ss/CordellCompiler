function foo() {
    glob arr smth[10, i8];
    smth[0] = 1;
}

function bar() {
    glob arr smth[10, i8];
    smth[0] = 1;
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            arrg %0 = arr_alloc(csu8 10);
            i8t* %2 = &(arrg %0);
            u64t %4 = i8n 0 as u64;
            u64t %3 = u64t %4 * csu8 1;
            i8t* %6 = u64t %3 as *ptr;
            i8t* %5 = i8t* %2 + i8t* %6;
            *(i8t* %5) = i8n 1;
        }
    }
    fn bar1()
    {
        {
            arrg %1 = arr_alloc(csu8 10);
            i8t* %7 = &(arrg %1);
            u64t %9 = i8n 0 as u64;
            u64t %8 = u64t %9 * csu8 1;
            i8t* %11 = u64t %8 as *ptr;
            i8t* %10 = i8t* %7 + i8t* %11;
            *(i8t* %10) = i8n 1;
        }
    }
}
/:
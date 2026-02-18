{
    start() {
        ptr ptr ptr i8 a;
        i8 b = a[0][0][0];
    }
}

: OUTPUT
{
    start {
        {
            {
                i8s*** %0 = alloc(8);
                i8s %1 = alloc(8);
                i8t*** %2 = i8s*** %0;
                i64t %3 = num? 0 * cnst? 1;
                i8t*** %5 = i64t %3 as *ptr;
                i8t*** %4 = i8t*** %2 + i8t*** %5;
                i8t** %6 = *(i8t*** %4);
                i8t** %7 = i8t** %6;
                i64t %8 = num? 0 * cnst? 1;
                i8t** %10 = i64t %8 as *ptr;
                i8t** %9 = i8t** %7 + i8t** %10;
                i8t* %11 = *(i8t** %9);
                i8t* %12 = i8t* %11;
                i64t %13 = num? 0 * cnst? 1;
                i8t* %15 = i64t %13 as *ptr;
                i8t* %14 = i8t* %12 + i8t* %15;
                i8t %16 = *(i8t* %14);
                i8s %1 = i8t %16;
            }
        }
    }
}
:
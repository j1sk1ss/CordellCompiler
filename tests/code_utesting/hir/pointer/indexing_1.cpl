{
    start() {
        ptr ptr ptr i8 a;
        ptr ptr i8 b = a[0];
        ptr i8 c = b[0];
        i8 d = c[0];
    }
}

: OUTPUT
{
    start {
        {
            {
                i8s*** %0 = alloc(8);
                i8s** %1 = alloc(8);
                i8t*** %4 = i8s*** %0;
                i64t %5 = num? 0 * cnst? 1;
                i8t*** %7 = i64t %5 as *ptr;
                i8t*** %6 = i8t*** %4 + i8t*** %7;
                i8t** %8 = *(i8t*** %6);
                i8s** %1 = i8t** %8;
                i8s* %2 = alloc(8);
                i8t** %9 = i8s** %1;
                i64t %10 = num? 0 * cnst? 1;
                i8t** %12 = i64t %10 as *ptr;
                i8t** %11 = i8t** %9 + i8t** %12;
                i8t* %13 = *(i8t** %11);
                i8s* %2 = i8t* %13;
                i8s %3 = alloc(8);
                i8t* %14 = i8s* %2;
                i64t %15 = num? 0 * cnst? 1;
                i8t* %17 = i64t %15 as *ptr;
                i8t* %16 = i8t* %14 + i8t* %17;
                i8t %18 = *(i8t* %16);
                i8s %3 = i8t %18;
            }
        }
    }
}
:
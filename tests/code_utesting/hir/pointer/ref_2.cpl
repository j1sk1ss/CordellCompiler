{
    start() {
        i8 a;
        f64 b;
        arr c[10, ptr i32];

        ptr ptr i32 d = ref c;
        ptr f64 e = ref b;
        ptr i8 f = ref a;
    }
}

: OUTPUT
{
    start {
        {
            {
                i8s %0 = alloc(8);
                f64s %1 = alloc(8);
                arrs* %2 = arr_alloc(num? 10);
                i32s** %3 = alloc(8);
                i32t** %6 = &(arrs* %2);
                i32s** %3 = i32t** %6;
                f64s* %4 = alloc(8);
                f64t* %7 = &(f64s %1);
                f64s* %4 = f64t* %7;
                i8s* %5 = alloc(8);
                i8t* %8 = &(i8s %0);
                i8s* %5 = i8t* %8;
            }
        }
    }
}
:
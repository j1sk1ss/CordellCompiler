start() {
    i32 a = 10;
    @[counter(10)] loop {
    }
    i32 b = 10;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = num? 10 as i32;
                i32s %6 = i32t %2;
                i64s %8 = alloc(8);
                i64s %7 = cnst? 10;
                future: i64s %8 = previous: i64s %7;
                i32t %5 = num? 10 as i32;
            }
            lb{X}
            [i64s %3] i64s %8 = phi(set [{X}][{X}]);
            goto lb{X}
            lb{X}
            {
                i64t %4 = i64s %8 - cnst? 1;
                i64s %9 = i64t %4;
                future: i64s %8 = previous: i64s %9;
                if i64t %4, goto lb{X} else goto lb{X}
            }
            lb{X}
            i32s %1 = alloc(8);
            i32s %10 = i32t %5;
        }
    }
}
/:

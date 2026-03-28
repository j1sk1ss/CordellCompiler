start() {
    i32 a = 1 as i32;
    if 1; {
        a = 1 as i32;
    }
    i32 b = a;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = num? 1 as i32;
                i32s %4 = i32t %2;
                future{X} i32s %5 = previous{X} i32s %4;
                if num? 1, goto lb{X}, else goto lb{X};
                lb{X}
                {
                    i32t %3 = num? 1 as i32;
                    i32s %7 = i32t %3;
                }
                future{X} i32s %5 = previous{X} i32s %7;
                goto lb{X}
                lb{X}
                [i32s %0] i32s %5 = phi(set [{X}][{X}]);
                i32s %1 = alloc(8);
                i32s %6 = i32s %5;
            }
        }
    }
}
/:

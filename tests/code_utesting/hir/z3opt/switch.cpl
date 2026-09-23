function foo();
function bar();
function baz();
start() {
    i32 a = 1;
    @[no_fall] @[straight]
    switch a; {
        case 1; foo();
        case 2; {
            i32 v = 100 * 1 + 1;
            v -= 1;
            bar();
        }
        default {
            baz(); baz(); baz();
        }
    }
    exit 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i32t %2 = i8n 1 as i32;
            i32s %15 = i32t %2;
            goto lb10;
            lb12:
            {
                foo0();
            }
            goto lb11;
            lb16:
            [unused] {
                [unused] i32s %1 = alloc;
                [unused] i8t %3 = i8n 100 * i8n 1;
                [unused] i8t %4 = i8t %3 + i8n 1;
                [unused] i32t %5 = i8t %4 as i32;
                [unused] i32s %16 = i32t %5;
                [unused] i32t %7 = i8n 1 as i32;
                [unused] i32t %6 = i32s %16 - i32t %7;
                [unused] i32s %17 = i32t %6;
                [unused] bar1();
            [unused] }
            [unused] [SSA] future: i32s %18 <<== previous: i32s %17;
            [unused] goto lb11;
            lb35:
            [unused] {
                [unused] baz2();
                [unused] baz2();
                [unused] baz2();
            [unused] }
            [unused] goto lb11;
            [unused] goto lb11;
            lb10:
            i64t %9 = csu8 1 as i64;
            i64t %10 = i32s %15 as i64;
            i8t %8 = i64t %10 == i64t %9;
            goto lb12;
            lb45:
            [unused] i64t %12 = csu8 2 as i64;
            [unused] i64t %13 = i32s %15 as i64;
            [unused] i8t %11 = i64t %13 == i64t %12;
            [unused] goto lb16;
            lb50:
            [unused] goto lb35;
            lb11:
            [base: i32s %1] i32s %18 = phi(set [%17, bb3]);
            u8t %14 = i8n 0 as u8;
            exit u8t %14;
        }
    }
}
/:
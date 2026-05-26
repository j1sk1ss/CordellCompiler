function sum(i32 a, i32 b) -> i32;
function sum(f64 a, f64 b) -> f64 {
    return sum(a as i32, b as i32) as f64;
}

start() {
    exit sum(10.0, 10.0) as i8;
}

:/ OUTPUT
{
    fn sum1(f64 a, f64 b) -> f64
    {
        f64s %2 = alloc;
        f64s %2 = load_arg();
        f64s %3 = alloc;
        f64s %3 = load_arg();
        {
            i32t %5 = f64s %2 as i32;
            use i32t %5;
            i32t %6 = f64s %3 as i32;
            use i32t %6;
            i32t %7 = sum0(i32t %5, i32t %6);
            f64t %4 = i32t %7 as f64;
            return f64t %4;
        }
    }
    fn _main()
    {
        {
            f64t %9 = sum1(f64n 4621819117588971520, f64n 4621819117588971520);
            i8t %8 = f64t %9 as i8;
            u8t %10 = i8t %8 as u8;
            exit u8t %10;
        }
    }
}
/:
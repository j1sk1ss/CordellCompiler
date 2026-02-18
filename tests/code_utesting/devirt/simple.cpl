{
    function sum(i32 a, i32 b) -> i32;
    function sum(f64 a, f64 b) -> f64 {
        return sum(a as i32, b as i32) as f64;
    }

    start() {
        exit sum(10.0, 10.0);
    }
}

: OUTPUT
{
    fn sum1(f64 a, f64 b) -> f64
    {
        f64s %2 = alloc(8);
        f64s %2 = load_arg();
        f64s %3 = alloc(8);
        f64s %3 = load_arg();
        {
            i32t %5 = f64s %2 as i32;
            use i32t %5;
            i32t %6 = f64s %3 as i32;
            use i32t %6;
            i32t %7 = call sum0(i32 a, i32 b) -> i32, argc args(i32t %5,i32t %6,);
            f64t %4 = i32t %7 as f64;
            return f64t %4;
        }
    }
    start {
        {
            {
                use f64n 4621819117588971520;
                use f64n 4621819117588971520;
                f64t %8 = call sum1(f64 a, f64 b) -> f64, argc args(f64n 4621819117588971520,f64n 4621819117588971520,);
                exit f64t %8;
            }
        }
    }
}
:
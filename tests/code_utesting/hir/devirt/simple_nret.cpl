function foo() {
    10;
}
start() {
    exit foo();
}

: OUTPUT
{
    fn foo0()
    {
        {
            return num? 10;
        }
    }
    start {
        {
            {
                i64t %0 = call foo0(), argc args();
                exit i64t %0;
            }
        }
    }
}
:
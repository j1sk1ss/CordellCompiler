{
    function foo();
    start() {
        loop foo();
    }
}

: OUTPUT
{
    start {
        {
            {
                lb4:
                goto lb5;
                lb5:
                {
                    call foo0(), argc args();
                }
                goto lb4;
                lb6:
            }
        }
    }
}
:
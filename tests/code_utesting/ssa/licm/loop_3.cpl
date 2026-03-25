function foo();
start() {
    loop foo();
}

: OUTPUT
{
    start {
        {
            {
                lb{X}
                goto lb{X}
                lb{X}
                {
                    call foo0(), argc args();
                }
                goto lb{X}
                lb{X}
            }
        }
    }
}
:
start() {
    i32 a;
    loop {
        a = 10 + 10;
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i8t %1 = i8n 10 + i8n 10;
                i32t %2 = i8t %1 as i32;
                i32s %3 = i32t %2;
                [unused] lb9:
                [unused] [i32s %0] i32s %4 = phi(set [%3, bb3]);
                [unused] goto lb10;
                [unused] lb10:
                [unused] {
                    [unused]
                [unused] }
                [unused] future: i32s %4 = previous: i32s %3;
                [unused] goto lb9;
                lb11:
            [unused] }
        [unused] }
    [unused] }
}
/:
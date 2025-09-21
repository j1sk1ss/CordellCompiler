{
    function foo(i32 a) => i32 {
        return a;
    }
    
    start(i64 argc, ptr u64 argv) {
        i32 a = 0;
        asm (a) {
            "mov {a}, rax"
        }

        {
            i32 b = 10;
        }
        i32 b = 11;

        exit foo(10);
    }
}
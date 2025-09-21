{
    function foo(i32 a) => i32 {
        return a;
    }
    
    start(i64 argc, ptr u64 argv) {
        i32 a = 0;
        asm (a) {
            "mov {a}, rax"
        }
        exit foo(10);
    }
}
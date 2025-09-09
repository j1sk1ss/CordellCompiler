{
    start(i64 argc, ptr u64 argv) {
        ptr str a1  = "Hello, World!";
        glob str b2 = "Hello, World!";
        ptr str c1  = "Goodbye, World!";
        exit 0;
    }
}

: Expected
{
    glob str str_0 = "Hello, World!"
    glob str str_1 = "Goodbye, World!"
    start(i64 argc, ptr u64 argv) {
        ptr str a1  = str_0;
        glob str b2 = str_0;
        ptr str c1  = str_1;
        exit 0;
    }
}
:
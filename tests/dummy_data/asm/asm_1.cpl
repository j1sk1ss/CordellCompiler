{

    start(i64 argc, ptr u64 argv) {
        i32 pos = 0;
        glob arr a[100, i8];

        switch argc; {
            case '*'; { syscall(1, 1, a + pos, 1); }
            case '.'; { syscall(0, 0, ref a[pos], 1); }
            case '<'; { pos -= 1; }
            case '>'; { pos += 1; }
            case '+'; { a[pos] += 1; }
        }

        exit 0;
    }
}
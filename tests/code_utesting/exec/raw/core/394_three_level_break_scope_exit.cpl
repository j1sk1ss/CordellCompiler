: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 total = 0;
    i32 outer = 0;
    while outer < 3; {
        i32 middle = 0;
        while middle < 4; {
            i32 inner = 0;
            while inner < 9; {
                total += 1;
                if inner == 1; { break; }
                inner += 1;
            }
            middle += 1;
        }
        outer += 1;
    }
    exit (total + 18) as u8;
}

:/ OUTPUT
@exit_code=42
/:

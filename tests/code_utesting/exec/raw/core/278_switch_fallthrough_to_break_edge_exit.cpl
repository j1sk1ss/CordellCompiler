: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 x = 1;
    i32 value = 0;

    switch x; {
        case 1; {
            value += 10;
        }
        case 2; {
            value += 32;
            break;
        }
        default {
            value = 0;
        }
    }

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 x = 2;
    i32 acc = 0;

    switch x; {
        case 1; {
            acc += 1;
        }
        case 2; {
            acc += 2;
        }
        case 3; {
            acc += 4;
            break;
        }
        default {
            acc += 8;
        }
    }

    exit acc as u8;
}

:/ OUTPUT
@exit_code=6
/:

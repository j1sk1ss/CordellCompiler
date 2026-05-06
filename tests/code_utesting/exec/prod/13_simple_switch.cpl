: RUN_ASM[args="+>"] :
start(i32 argc, ptr ptr i8 argv) {
    i32 pc = 0;

    while pc < 2; {
        @[no_fall]
        switch argv[1][pc]; {
            case '+'; {
                pc += 1;
            }
            case '>'; {
                pc += 1;
            }
            default {
                exit 100;
            }
        }
    }

    exit pc;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=2
/:
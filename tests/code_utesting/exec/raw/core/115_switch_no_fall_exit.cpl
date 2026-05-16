: RUN_ASM :

start() {
    i32 out = 0;

    @[no_fall] switch 3; {
        case 1; { out = 11; }
        case 2; { out = 17; }
        case 3; { out = 21; }
        default { out = 99; }
    }

    exit out as u8;
}

:/ OUTPUT
@exit_code=21
/:

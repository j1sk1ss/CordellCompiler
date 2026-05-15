: RUN_ASM :

start() {
    i8 a = 'A';
    i8 b = 'B';
    arr ps[2, ptr i8] = { ref a, ref b };

    exit (dref ps[0] + dref ps[1]) as u8;
}

:/ OUTPUT
@exit_code=131
/:

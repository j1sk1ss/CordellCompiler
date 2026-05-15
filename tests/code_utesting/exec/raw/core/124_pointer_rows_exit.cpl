: RUN_ASM :

start() {
    arr left[3, i8] = { 'A', 'B', 'C' };
    arr right[3, i8] = { 'D', 'E', 'F' };
    arr rows[2, ptr i8] = { ref left, ref right };

    exit (rows[1][0] + rows[0][1]) as u8;
}

:/ OUTPUT
@exit_code=134
/:

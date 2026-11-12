: BLOCK_TEST :
: RUN_ASM :

start() {
    arr matrix[3, arr[4, i32]];
    matrix[0][0] = 7;
    matrix[0][3] = 11;
    matrix[2][0] = 13;
    matrix[2][3] = 17;
    exit (matrix[0][0] + matrix[0][3] + matrix[2][0] + matrix[2][3] - 6) as u8;
}

:/ OUTPUT
@exit_code=42
/:

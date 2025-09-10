{
    start(i64 argc, ptr u64 argv) {
        i8 a = 126;  : Ok :
        i8 a1 = 128; : Error :
        u8 a2 = 128; : Ok :
        u8 a3 = 256; : Error :

        arr a4[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };   : Ok :
        arr a5[10, i8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 128 }; : Error :
        arr a6[10, u8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 128 }; : Ok :
        arr a7[5, u8] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };    : Error :
        exit 0;
    }
}
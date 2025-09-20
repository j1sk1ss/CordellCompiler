{
    start(i64 argc, ptr i64 argv) {
        : 24 : str stack_str = "String value";
        : 32 : ptr str str_ptr = ref stack_str;
        : 40 : ptr u8 str_ptr_ptr = ref str_ptr;

        : 48 : i64 a = 0x0;
        : 56 : i32 b = 0b1;
        : 64 : i16 c = 2;
        : 72 : i8 d  = 'a';

        a = b;
        c = b;
        d = c;
        
        d = str_ptr_ptr;
        str_ptr_ptr = c;

        : 24 : arr large_arr[5, i8] = {1,2,256,4,5,6,7,8,9,10};
        exit 0;
    }
}
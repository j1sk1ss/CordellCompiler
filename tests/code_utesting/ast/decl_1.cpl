{
    start() {
        i8 a;
        i16 b;
        i32 c;
        i64 d;
        a + b + c + d;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i16] (I16_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I16_VARIABLE_TOKEN, v_id=1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
         [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
            [d] (I64_VARIABLE_TOKEN, v_id=3, s_id=2)
         [+] (PLUS_TOKEN, v_id=-1, s_id=0)
            [+] (PLUS_TOKEN, v_id=-1, s_id=0)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [a] (I8_VARIABLE_TOKEN, v_id=0, s_id=2)
                  [b] (I16_VARIABLE_TOKEN, v_id=1, s_id=2)
               [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
            [d] (I64_VARIABLE_TOKEN, v_id=3, s_id=2)
:
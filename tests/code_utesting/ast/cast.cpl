{
    start() {
        u8 a = 10 as u8;
        i64 b = (10 as u8 + 11 as u8) as i64;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [u8] (U8_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (U8_VARIABLE_TOKEN, v_id=0, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
               [u8] (U8_TYPE_TOKEN, v_id=-1, s_id=0)
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I64_VARIABLE_TOKEN, v_id=1, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
               [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                     [u8] (U8_TYPE_TOKEN, v_id=-1, s_id=0)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                     [u8] (U8_TYPE_TOKEN, v_id=-1, s_id=0)
                     [11] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
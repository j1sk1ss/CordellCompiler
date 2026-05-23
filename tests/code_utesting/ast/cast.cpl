start() {
   u8 a = 10 as u8;
   i64 b = (10 as u8 + 11 as u8) as i64;
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [u8] (U8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [a] (U8_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [u8] (U8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i64] (I64_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [b] (I64_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
            [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i64] (I64_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [u8] (U8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [u8] (U8_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [11] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
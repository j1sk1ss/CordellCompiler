start() {
   i8 a;
   i16 b;
   i32 c;
   i64 d;
   a + b + c + d;
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I8_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
         [i16] (I16_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [b] (I16_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=5, s_id=-1)
            [c] (I32_VARIABLE_TOKEN, v_id=2, t_id=-1, s_id=2)
         [i64] (I64_TYPE_TOKEN, v_id=-1, t_id=7, s_id=-1)
            [d] (I64_VARIABLE_TOKEN, v_id=3, t_id=-1, s_id=2)
         [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [a] (I8_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                  [b] (I16_VARIABLE_TOKEN, v_id=1, t_id=3, s_id=2)
               [c] (I32_VARIABLE_TOKEN, v_id=2, t_id=5, s_id=2)
            [d] (I64_VARIABLE_TOKEN, v_id=3, t_id=7, s_id=2)
/:
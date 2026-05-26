start() {
   ptr i8 buffer;
   buffer[10 + 1] = 1;
   buffer[buffer[1 + dref buffer]] = 1;
   dref buffer = buffer[0];
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
            [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [[]]
               [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [[]]
               [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
               [[]]
                  [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
                  [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            [[]]
               [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
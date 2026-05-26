start() {
   10();
   123();
   (123)();
   -1();
   ptr i32 a;
   a();
   (8 * 8)();
   (dref a)();
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [()]
            [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [()]
            [123] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [()]
            [123] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [()]
            [-1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
         [()]
            [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            { scope, id=-1 }
         [()]
            [*] (MULTIPLY_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [8] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [8] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [()]
            [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
            { scope, id=-1 }
/:
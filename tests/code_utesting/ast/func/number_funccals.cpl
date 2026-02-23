{
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
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [()]
            [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
         [()]
            [123] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
         [()]
            [123] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
         [()]
            [-1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
         [()]
            [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
            { scope, id=0 }
         [()]
            [*] (MULTIPLY_TOKEN, v_id=-1, s_id=0)
               [8] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [8] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
         [()]
            [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
            { scope, id=0 }
:
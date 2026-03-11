{
    start() {
        arr a[10, ptr i32];
        i32 b = @[sizeof]a;
        i32 c = @[sizeof]a[0];
        i32 d = @[sizeof]a[0][0];
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [arr] (ARRAY_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (ARR_VARIABLE_TOKEN, v_id=0, s_id=2)
            [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
            [a] (ARR_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
            [[]]
               [a] (ARR_VARIABLE_TOKEN, v_id=0, s_id=2)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [d] (I32_VARIABLE_TOKEN, v_id=3, s_id=2)
            [[]]
               [[]]
                  [a] (ARR_VARIABLE_TOKEN, v_id=0, s_id=2)
                  [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
{
    function min(i32 a, i32 b) -> i32;
    function max(i32 a, i32 b) -> i32;
    function logic(i32 a, i32 b, ptr u64 perform) -> i32 {
        return perform(a, b);
    }

    start() {
        logic(10, 20, min);
        logic(10, 20, max);
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [min] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [max] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=2, s_id=3)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [logic] (FUNC_NAME_TOKEN, v_id=2, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=4, s_id=4)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=5, s_id=4)
         [u64] (U64_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [perform] (U64_VARIABLE_TOKEN, ptr, v_id=6, s_id=4)
         { scope, id=5 }
            [return] (RETURN_TOKEN, v_id=-1, s_id=5)
               [()]
                  [perform] (U64_VARIABLE_TOKEN, ptr, v_id=6, s_id=4)
                  { scope, id=0 }
                     [a] (I32_VARIABLE_TOKEN, v_id=4, s_id=4)
                     [b] (I32_VARIABLE_TOKEN, v_id=5, s_id=4)
   [start] (START_TOKEN, v_id=3, s_id=1)
      { scope, id=6 }
         [logic] (CALL_TOKEN, v_id=2, s_id=0)
            { scope, id=0 }
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [20] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [min] (CALL_ADDR, v_id=0, s_id=0)
         [logic] (CALL_TOKEN, v_id=2, s_id=0)
            { scope, id=0 }
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [20] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [max] (CALL_ADDR, v_id=1, s_id=0)
:
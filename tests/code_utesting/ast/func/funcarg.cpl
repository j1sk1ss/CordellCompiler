{
    function a() -> i32;
    function b(i32 g) -> i32;
    function c(i32 f = b(a()));
    start() {
        c();
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [a] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [b] (FUNC_NAME_TOKEN, v_id=1, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [g] (I32_VARIABLE_TOKEN, v_id=0, s_id=3)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [c] (FUNC_NAME_TOKEN, v_id=2, s_id=0)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [f] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
            [b] (CALL_TOKEN, v_id=1, s_id=0)
               { scope, id=0 }
                  [a] (CALL_TOKEN, v_id=0, s_id=0)
                     { scope, id=0 }
   [start] (START_TOKEN, v_id=3, s_id=0)
      { scope, id=5 }
         [c] (CALL_TOKEN, v_id=2, s_id=0)
            { scope, id=0 }
               [b] (CALL_TOKEN, v_id=1, s_id=0)
                  { scope, id=0 }
                     [a] (CALL_TOKEN, v_id=0, s_id=0)
                        { scope, id=0 }
:
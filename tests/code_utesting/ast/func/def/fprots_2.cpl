{
    function foo(i32 a = 1);
    function foo(i32 a = 1) {
        a += 1;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         { scope, id=4 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
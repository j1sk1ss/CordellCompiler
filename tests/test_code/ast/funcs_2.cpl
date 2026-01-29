{
    function foo() => i0;
    function foo(i32 a) => i0;
    function foo(i32 a, i32 b) => i0;
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=1, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=3)
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=2, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=4)
:
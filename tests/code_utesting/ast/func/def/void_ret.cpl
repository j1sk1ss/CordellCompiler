{
    function foo() -> ptr i0 {
        return 0;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
      { scope, id=2 }
         { scope, id=3 }
            [return] (RETURN_TOKEN, v_id=-1, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
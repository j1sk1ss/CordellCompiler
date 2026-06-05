function foo(u32 a);
start() {
   ptr i0 a = foo;
   a(10);
   foo(10);
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [u32] (U32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (U32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=3 }
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, t_id=3, s_id=-1)
            [a] (I0_VARIABLE_TOKEN, ptr, v_id=1, t_id=-1, s_id=3)
            [foo] (CALL_ADDR_TOKEN, v_id=0, t_id=-1, s_id=1)
         [()]
            [a] (I0_VARIABLE_TOKEN, ptr, v_id=1, t_id=3, s_id=3)
            { scope, id=-1 }
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
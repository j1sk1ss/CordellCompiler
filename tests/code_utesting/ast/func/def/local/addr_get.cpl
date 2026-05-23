function foo() -> ptr u64 {
   function bar() {
      return 1;
   }
   return bar;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [u64] (U64_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
         { scope, id=3 }
            [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [bar] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=3)
               { scope, id=4 }
                  { scope, id=5 }
                     [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=5)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [bar] (CALL_ADDR_TOKEN, v_id=1, t_id=-1, s_id=3)
/:
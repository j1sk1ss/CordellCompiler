{
    function foo() -> ptr i0 {
        function bar();
        function baz() {
            return bar();
        }
        function bar() {
            return 10;
        }
        return baz;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
      { scope, id=2 }
         { scope, id=3 }
            [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
               [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=3)
               { scope, id=4 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [baz] (FUNC_NAME_TOKEN, v_id=2, s_id=3)
               { scope, id=5 }
                  { scope, id=6 }
                     [return] (RETURN_TOKEN, v_id=-1, s_id=6)
                        [bar] (CALL_TOKEN, v_id=1, s_id=0)
                           { scope, id=0 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=3)
               { scope, id=7 }
                  { scope, id=8 }
                     [return] (RETURN_TOKEN, v_id=-1, s_id=8)
                        [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [return] (RETURN_TOKEN, v_id=-1, s_id=3)
               [baz] (CALL_ADDR_TOKEN, v_id=2, s_id=0)
:
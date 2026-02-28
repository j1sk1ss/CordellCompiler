{
    function foo(i32 a) {
        i32 b;
        function bar(i32 b) {
            b -= 1;
        }
        bar(b);
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         { scope, id=3 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=3)
               { scope, id=4 }
                  [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                     [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=4)
                  { scope, id=5 }
                     [-=] (SUBASSIGN_TOKEN, v_id=-1, s_id=0)
                        [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=4)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [bar] (CALL_TOKEN, v_id=1, s_id=0)
               { scope, id=0 }
                  [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
:
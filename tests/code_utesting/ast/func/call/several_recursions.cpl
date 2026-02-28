{
    function foo(i32 a) {
        foo(a + 1);
    }

    function bar(i32 a) {
        bar(a + 1);
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
            [foo] (CALL_TOKEN, v_id=0, s_id=0)
               { scope, id=0 }
                  [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                     [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=1)
      { scope, id=4 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
         { scope, id=5 }
            [bar] (CALL_TOKEN, v_id=1, s_id=0)
               { scope, id=0 }
                  [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                     [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=4)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
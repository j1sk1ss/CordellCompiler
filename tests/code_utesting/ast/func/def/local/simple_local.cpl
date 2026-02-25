{
    function foo() {
        function bar() {
        }
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [function] (FUNC_TOKEN, v_id=-1, s_id=0)
               [bar] (FUNC_NAME_TOKEN, v_id=1, s_id=3)
               { scope, id=4 }
                  { scope, id=5 }
:
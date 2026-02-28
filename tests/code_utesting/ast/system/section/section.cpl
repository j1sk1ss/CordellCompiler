{
    section(".text") {
        glob i32 a = 10;
        function foo() {
            return 1;
        }
    }
}

: OUTPUT
{ scope, id=1 }
   { scope, id=1 }
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
         [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1, glob)
         [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
      [function] (FUNC_TOKEN, v_id=-1, s_id=0)
         [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         { scope, id=2 }
            { scope, id=3 }
               [return] (RETURN_TOKEN, v_id=-1, s_id=3)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
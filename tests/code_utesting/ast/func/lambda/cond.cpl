start() {
    i32 a = ((i32 a, i32 b) => { if 1; return a + b; else return a; })(10, 10);
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
            [()]
               [lambda]
                  { scope, id=3 }
                     [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                        [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
                     [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                        [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=3)
                     { scope, id=4 }
                        [if] (IF_TOKEN, v_id=-1, s_id=4)
                           [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                           { scope, id=5 }
                              [return] (RETURN_TOKEN, v_id=-1, s_id=5)
                                 [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                                    [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
                                    [b] (I32_VARIABLE_TOKEN, v_id=2, s_id=3)
                           { scope, id=4 }
                              [return] (RETURN_TOKEN, v_id=-1, s_id=4)
                                 [a] (I32_VARIABLE_TOKEN, v_id=1, s_id=3)
               { scope, id=0 }
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
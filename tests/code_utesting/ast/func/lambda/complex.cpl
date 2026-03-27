start() {
    exit ((i32 a) => {
        while a; {
            a -= 1;
        }
        return 0;
    })(10);
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [exit] (EXIT_TOKEN, v_id=-1, s_id=2)
            [()]
               [lambda]
                  { scope, id=3 }
                     [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                        [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=3)
                     { scope, id=4 }
                        [while] (WHILE_TOKEN, v_id=-1, s_id=4)
                           [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=3)
                           { scope, id=5 }
                              [-=] (SUBASSIGN_TOKEN, v_id=-1, s_id=0)
                                 [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=3)
                                 [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                        [return] (RETURN_TOKEN, v_id=-1, s_id=4)
                           [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               { scope, id=0 }
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
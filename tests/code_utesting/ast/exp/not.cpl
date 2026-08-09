start() {
   i32 a = not 1;
   if not (not (not (not a))); {
      a = not not not not a;
   }
   exit not (1 + 1 + 1 * (not a));
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [if] (IF_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
            { scope, id=3 }
               [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                  [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                           [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                              [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
         [exit] (EXIT_TOKEN, v_id=-1, t_id=-1, s_id=2)
            [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [*] (MULTIPLY_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
/:
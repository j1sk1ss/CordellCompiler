{
    start() {
        while 1; break;
        while 0; exit 1;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         [while] (WHILE_TOKEN, v_id=-1, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=3 }
               [break] (BREAK_TOKEN, v_id=-1, s_id=0)
         [while] (WHILE_TOKEN, v_id=-1, s_id=2)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=4 }
               [exit] (EXIT_TOKEN, v_id=-1, s_id=4)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
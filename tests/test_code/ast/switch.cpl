{
    start() {
        switch 1; {
            case 1; {
            }
            case 2; {
                break;
            }
            default {
            }
        }
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [switch] (SWITCH_TOKEN, v_id=0, s_id=2)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
               [case] (CASE_TOKEN, v_id=-1, s_id=0)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  { scope, id=3 }
               [case] (CASE_TOKEN, v_id=-1, s_id=0)
                  [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  { scope, id=4 }
                     [break] (BREAK_TOKEN, v_id=-1, s_id=0)
               [default] (DEFAULT_TOKEN, v_id=-1, s_id=0)
                  { scope, id=5 }
:
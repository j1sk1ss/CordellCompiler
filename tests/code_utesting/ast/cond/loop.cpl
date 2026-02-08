{
    start() {
        loop {
            break;
        }

        loop break;
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [loop] (LOOP_TOKEN, v_id=0, s_id=2)
            { scope, id=3 }
               [break] (BREAK_TOKEN, v_id=-1, s_id=0)
         [loop] (LOOP_TOKEN, v_id=0, s_id=2)
            { scope, id=4 }
               [break] (BREAK_TOKEN, v_id=-1, s_id=0)
:
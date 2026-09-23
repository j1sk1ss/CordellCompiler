interface a {
}
container b::a {
}

start() {
    ptr i0 addr = 0x0;
    ptr b instance = place(addr, b);
}


:/ OUTPUT
{ scope, id=1 }
   [interface] (, v_id=-1, t_id=-1, s_id=-1)
      [a] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=2 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [b] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=1, s_id=1)
      { scope, id=3 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=4 }
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, t_id=3, s_id=-1)
            [addr] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=4)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [b] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=4, s_id=-1)
            [instance] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=1, t_id=4, s_id=4)
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [place] (PLACE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [addr] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=3, s_id=4)
                  [b] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=1, s_id=-1)
/:
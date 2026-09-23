start() {
   ptr fn()i0 fnc;
   fnc();
   ptr i0 a;
   a();
}

:/ OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [fn] (SIGNATURE_TOKEN, ptr, v_id=-1, t_id=2, s_id=-1)
            [fnc] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
         [()]
            [fnc] (I0_VARIABLE_TOKEN, ptr, v_id=0, t_id=2, s_id=2)
            { scope, id=-1 }
         [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, t_id=3, s_id=-1)
            [a] (I0_VARIABLE_TOKEN, ptr, v_id=1, t_id=3, s_id=2)
         [()]
            [a] (I0_VARIABLE_TOKEN, ptr, v_id=1, t_id=3, s_id=2)
            { scope, id=-1 }
/:
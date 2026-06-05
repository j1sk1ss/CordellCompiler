container storage {
    function init(ptr storage self) -> i0 {
    }
}

start() {
    storage s;
    ptr storage l;
    s.init();
    l.init();
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
         [storage] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=2, s_id=-1)
            [self] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=0, t_id=1, s_id=2)
         { scope, id=3 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [storage] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=4 }
         [storage] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=4, s_id=-1)
            [s] (CUSTOM_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=4)
         [storage] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=5, s_id=-1)
            [l] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=2, t_id=1, s_id=4)
         [()]
            [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [()]
            [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=-1)
            { scope, id=-1 }
/:
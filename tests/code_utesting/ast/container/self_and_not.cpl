container node {
    function init(ptr node self) {
    }

    @[static]
    function static() {
    }
}

start() {
    node nd;
    nd.init();
    nd.static();
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [static] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=4 }
         { scope, id=5 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [node] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=2, s_id=-1)
            [self] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=0, t_id=1, s_id=2)
         { scope, id=3 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
   [start] (START_TOKEN, v_id=2, t_id=-1, s_id=1)
      { scope, id=6 }
         [node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=5, s_id=-1)
            [nd] (CUSTOM_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=6)
         [()]
            [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=-1)
            { scope, id=-1 }
         [()]
            [static] (FUNC_NAME_TOKEN, v_id=1, t_id=-1, s_id=-1)
            { scope, id=-1 }
/:
container nested {
    function init(ptr nested self) {
    }
}

container wrapper {
    nested stack;
    ptr nested pointer;
}

start() {
    wrapper w;
    w.stack.init();
    w.pointer.init();
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [nested] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [wrapper] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=2, s_id=1)
      { scope, id=1 }
         [nested] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [stack] (CUSTOM_VARIABLE_TOKEN, v_id=1, t_id=0, s_id=1)
         [nested] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=4, s_id=-1)
            [pointer] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=2, t_id=0, s_id=1)
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=4 }
         [wrapper] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [w] (CUSTOM_VARIABLE_TOKEN, v_id=3, t_id=2, s_id=4)
         [()]
            [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=-1)
            { scope, id=-1 }
               [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [., type=3]
                     [w] (CUSTOM_VARIABLE_TOKEN, v_id=3, t_id=2, s_id=4)
                     [stack] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=3, s_id=-1)
         [()]
            [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=-1)
            { scope, id=-1 }
               [., type=4]
                  [w] (CUSTOM_VARIABLE_TOKEN, v_id=3, t_id=2, s_id=4)
                  [pointer] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=4, s_id=-1)
   [function] (FUNC_TOKEN, v_id=-1, t_id=1, s_id=-1)
      [init] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [nested] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=0, s_id=-1)
            [self] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=0, t_id=0, s_id=2)
         { scope, id=3 }
/:
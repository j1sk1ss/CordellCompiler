container node {
    ptr i32 p;
    i32 size;
}

function foo(ptr node nd) -> i0 {
    nd.p = 0;
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, t_id=2, s_id=-1)
            [p] (I32_VARIABLE_TOKEN, ptr, v_id=0, t_id=-1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=4, s_id=-1)
            [size] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=3 }
         [node] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=6, s_id=-1)
            [nd] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=2, t_id=5, s_id=3)
         { scope, id=4 }
            [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [., type=2]
                  [nd] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=2, t_id=6, s_id=3)
                  [p] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=2, s_id=-1)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
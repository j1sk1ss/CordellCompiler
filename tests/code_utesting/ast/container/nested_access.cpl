container a {
    i32 field;
}

container b {
    ptr a field;
}

start() {
    b nd;
    nd.field.field = 0;
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [a] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [field] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1)
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [b] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=2, s_id=1)
      { scope, id=1 }
         [a] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=3, s_id=-1)
            [field] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=1, t_id=0, s_id=1)
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [b] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [nd] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=2, s_id=2)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [., type=1]
               [., type=3]
                  [nd] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=2, s_id=2)
                  [field] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=3, s_id=-1)
               [field] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
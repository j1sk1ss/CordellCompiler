container node {
    i32 a;
    ptr u8 b;
}

start() {
    node nd;
    nd.a = 1;
    i32 b = nd.b as i32 + nd.a;
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1)
         [u8] (U8_TYPE_TOKEN, ptr, v_id=-1, t_id=2, s_id=-1)
            [b] (U8_VARIABLE_TOKEN, ptr, v_id=1, t_id=-1, s_id=1)
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [nd] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [., type=1]
               [nd] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
               [a] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=3, t_id=-1, s_id=2)
            [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [., type=2]
                     [nd] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
                     [b] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=2, s_id=-1)
               [., type=1]
                  [nd] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
                  [a] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=1, s_id=-1)
/:
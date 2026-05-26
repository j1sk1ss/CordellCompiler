container tuple {
   i32 a;
   i32 b;
}

start() {
   tuple b;
   b;
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [tuple] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=1 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
            [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=1)
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         [tuple] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
            [b] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
         [b] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
/:
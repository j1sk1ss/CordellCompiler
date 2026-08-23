container chain_node {
   ptr chain_node next;
   i32 value;
}

container chain_pair {
   chain_node first;
   chain_node second;
}

start() {
   chain_pair p;
   p.first.value  = 17;
   p.second.value = 25;
}

:/ OUTPUT
{ scope, id=1 }
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [chain_node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=0, s_id=1)
      { scope, id=2 }
         [chain_node] (CUSTOM_TYPE_TOKEN, ptr, v_id=-1, t_id=1, s_id=-1)
            [next] (CUSTOM_VARIABLE_TOKEN, ptr, v_id=0, t_id=1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [value] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=2)
   [container] (, v_id=-1, t_id=-1, s_id=-1)
      [chain_pair] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=4, s_id=1)
      { scope, id=3 }
         [chain_node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=5, s_id=-1)
            [first] (CUSTOM_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=3)
         [chain_node] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=6, s_id=-1)
            [second] (CUSTOM_VARIABLE_TOKEN, v_id=3, t_id=0, s_id=3)
   [start] (START_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=4 }
         [chain_pair] (CUSTOM_TYPE_TOKEN, v_id=-1, t_id=7, s_id=-1)
            [p] (CUSTOM_VARIABLE_TOKEN, v_id=4, t_id=4, s_id=4)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [., type=3]
               [., type=5]
                  [p] (CUSTOM_VARIABLE_TOKEN, v_id=4, t_id=7, s_id=4)
                  [first] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=5, s_id=-1)
               [value] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [17] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [., type=3]
               [., type=6]
                  [p] (CUSTOM_VARIABLE_TOKEN, v_id=4, t_id=7, s_id=4)
                  [second] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=6, s_id=-1)
               [value] (UNKNOWN_STRING_TOKEN, v_id=-1, t_id=3, s_id=-1)
            [25] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
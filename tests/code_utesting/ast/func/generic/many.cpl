function foo<O,R,E,G,N>() {
    O a;
    R b;
    E c;
    G d;
    O e;
    N f;
}

start() {
    foo<i32,i32,i32,i32,i32>();
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [O] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               [a] (GENERIC_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=3)
            [R] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               [b] (GENERIC_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3)
            [E] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
               [c] (GENERIC_VARIABLE_TOKEN, v_id=2, t_id=-1, s_id=3)
            [G] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
               [d] (GENERIC_VARIABLE_TOKEN, v_id=3, t_id=-1, s_id=3)
            [O] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               [e] (GENERIC_VARIABLE_TOKEN, v_id=4, t_id=-1, s_id=3)
            [N] (GENERIC_TYPE_TOKEN, v_id=-1, t_id=4, s_id=-1)
               [f] (GENERIC_VARIABLE_TOKEN, v_id=5, t_id=-1, s_id=3)
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=4 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            { scope, id=-1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=2, t_id=-1, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [O] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               [a] (I32_VARIABLE_TOKEN, v_id=11, t_id=-1, s_id=3)
            [R] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
               [b] (I32_VARIABLE_TOKEN, v_id=10, t_id=-1, s_id=3)
            [E] (I32_TYPE_TOKEN, v_id=-1, t_id=2, s_id=-1)
               [c] (I32_VARIABLE_TOKEN, v_id=9, t_id=-1, s_id=3)
            [G] (I32_TYPE_TOKEN, v_id=-1, t_id=3, s_id=-1)
               [d] (I32_VARIABLE_TOKEN, v_id=8, t_id=-1, s_id=3)
            [O] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
               [e] (I32_VARIABLE_TOKEN, v_id=7, t_id=-1, s_id=3)
            [N] (I32_TYPE_TOKEN, v_id=-1, t_id=4, s_id=-1)
               [f] (I32_VARIABLE_TOKEN, v_id=6, t_id=-1, s_id=3)
/:
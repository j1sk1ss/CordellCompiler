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
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [O] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
               [a] (GENERIC_VARIABLE_TOKEN, v_id=0, s_id=3)
            [R] (GENERIC_TYPE_TOKEN, v_id=1, s_id=0)
               [b] (GENERIC_VARIABLE_TOKEN, v_id=1, s_id=3)
            [E] (GENERIC_TYPE_TOKEN, v_id=2, s_id=0)
               [c] (GENERIC_VARIABLE_TOKEN, v_id=2, s_id=3)
            [G] (GENERIC_TYPE_TOKEN, v_id=3, s_id=0)
               [d] (GENERIC_VARIABLE_TOKEN, v_id=3, s_id=3)
            [O] (GENERIC_TYPE_TOKEN, v_id=0, s_id=0)
               [e] (GENERIC_VARIABLE_TOKEN, v_id=4, s_id=3)
            [N] (GENERIC_TYPE_TOKEN, v_id=4, s_id=0)
               [f] (GENERIC_VARIABLE_TOKEN, v_id=5, s_id=3)
   [start] (START_TOKEN, v_id=1, s_id=1)
      { scope, id=4 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
/:
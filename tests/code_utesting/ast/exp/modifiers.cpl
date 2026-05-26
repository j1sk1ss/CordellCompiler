glob function foo() -> i32;
start() {
   glob i32 a;
   glob ro i32 b;
   ro glob i32 c;
   ro i32 d;
   ro glob ptr i32 e;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1, glob)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, glob)
            [a] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=3, glob)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, ro, glob)
            [b] (I32_VARIABLE_TOKEN, v_id=1, t_id=-1, s_id=3, ro, glob)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, ro, glob)
            [c] (I32_VARIABLE_TOKEN, v_id=2, t_id=-1, s_id=3, ro, glob)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1, ro)
            [d] (I32_VARIABLE_TOKEN, v_id=3, t_id=-1, s_id=3, ro)
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1, ro, glob)
            [e] (I32_VARIABLE_TOKEN, ptr, v_id=4, t_id=-1, s_id=3, ro, glob)
/:
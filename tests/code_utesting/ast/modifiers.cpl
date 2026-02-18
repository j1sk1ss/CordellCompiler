{
    glob function foo() -> i32;
    start() {
        glob i32 a;
        glob ro i32 b;
        ro glob i32 c;
        ro i32 d;
        ro glob ptr i32 e;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0, glob)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
   [start] (START_TOKEN, v_id=1, s_id=0)
      { scope, id=3 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
            [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=3, glob)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, ro, glob)
            [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=3, ro, glob)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, ro, glob)
            [c] (I32_VARIABLE_TOKEN, v_id=2, s_id=3, ro, glob)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, ro)
            [d] (I32_VARIABLE_TOKEN, v_id=3, s_id=3, ro)
         [i32] (I32_TYPE_TOKEN, ptr, v_id=-1, s_id=0, ro, glob)
            [e] (I32_VARIABLE_TOKEN, ptr, v_id=4, s_id=3, ro, glob)
:
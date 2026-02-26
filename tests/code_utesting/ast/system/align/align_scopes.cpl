{
    align(16) {
        glob i32 a;
    }
    function foo() {
        a += 1;
    }
}

: OUTPUT
{ scope, id=1 }
   { scope, id=1 }
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, glob)
         [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1, glob)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
      { scope, id=2 }
         { scope, id=3 }
            [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1, glob)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
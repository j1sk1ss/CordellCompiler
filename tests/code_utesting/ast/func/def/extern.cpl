extern function foo();
start() {
   foo();
}

:/ OUTPUT
{ scope, id=1 }
   [extern] (EXTERN_TOKEN, v_id=-1, s_id=0)
      [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0, ext, glob)
         [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         { scope, id=2 }
   [start] (START_TOKEN, v_id=1, s_id=1)
      { scope, id=3 }
         [()]
            [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1, ext)
            { scope, id=0 }
/:
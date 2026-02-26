{
    section(".text") {
        glob ptr i0 printf;
        function foo();
    }
}

: OUTPUT
{ scope, id=1 }
   { scope, id=1 }
      [i0] (I0_TYPE_TOKEN, ptr, v_id=-1, s_id=0, glob)
         [printf] (I0_VARIABLE_TOKEN, ptr, v_id=0, s_id=1, glob)
      [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
         [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         { scope, id=2 }
:
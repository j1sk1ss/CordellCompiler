{
    function foo() -> ptr i8;
    start() {
        dref foo() = 1;
        dref (0x7c00 as ptr i8) = 2;
        dref (0xb840000 as ptr i8) = 'H';
        dref (1 + dref 0x1 - dref dref 0x01) = 1;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
      [foo] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
      { scope, id=2 }
   [start] (START_TOKEN, v_id=1, s_id=0)
      { scope, id=3 }
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
               [foo] (CALL_TOKEN, v_id=0, s_id=0)
                  { scope, id=0 }
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
                  [31744] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
                  [193200128] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [72] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                     [-] (MINUS_TOKEN, v_id=-1, s_id=0)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                        [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                           [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                              [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
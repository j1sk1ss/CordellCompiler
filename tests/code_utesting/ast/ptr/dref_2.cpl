function foo() -> ptr i8;
start() {
   dref foo() = 1;
   dref (0x7c00 as ptr i8) = 2;
   dref (0xb840000 as ptr i8) = 'H';
   dref (1 + dref 0x1 - dref dref 0x01) = 1;
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_PROT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=3 }
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [()]
                  [foo] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
                  { scope, id=-1 }
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
                  [31744] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [2] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=-1, s_id=-1)
                  [193200128] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [72] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
         [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [-] (MINUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
/:
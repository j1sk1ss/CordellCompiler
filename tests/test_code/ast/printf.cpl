{
    function _strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function _putc(i8 c) => i0 {
        syscall(0x2000004, 1, ref c, 1);
    }

    function _putn(i32 a) => i0 {
        while a > 0; {
            _putc(a % 10 + '0');
            a /= 10;
        }
    }

    function _puts(ptr i8 s) => i0 {
        while dref s; {
            _putc(dref s);
            s += 1;
        }
    }

    function printf(ptr i8 fmt, ...) {
        i32 index = 0;
        i64 fmt_len = _strlen(fmt);
        while index < fmt_len; {
            switch fmt[index]; {
                case '%'; {
                    i8 mode = fmt[index += 1];
                    switch mode; {
                        case 's'; {
                            _puts(poparg as ptr i8);
                            break;
                        }
                        case 'u'; {}
                        case 'i'; {}
                        case 'l'; {}
                        case 'd'; {
                            _putn(poparg as ptr i8);
                            break;
                        }
                    }

                    break;
                }
                default {
                    _putc(fmt[index]);
                    break;
                }
            }

            index += 1;
        }
    }

    start() {
        printf("Hello %s! Im %i yo!\n", "Max", 22);
        exit 0;
    }
}

: OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [_strlen] (FUNC_NAME_TOKEN, v_id=0, s_id=0)
         [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [s] (I8_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
         { scope, id=3 }
            [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
               [l] (I64_VARIABLE_TOKEN, v_id=1, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [while] (WHILE_TOKEN, v_id=0, s_id=3)
               [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                  [s] (I8_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
               { scope, id=4 }
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                     [s] (I8_VARIABLE_TOKEN, ptr, v_id=0, s_id=2)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                     [l] (I64_VARIABLE_TOKEN, v_id=1, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [return] (RETURN_TOKEN, v_id=0, s_id=3)
               [l] (I64_VARIABLE_TOKEN, v_id=1, s_id=3)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [_putc] (FUNC_NAME_TOKEN, v_id=1, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=5 }
         [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [c] (I8_VARIABLE_TOKEN, v_id=2, s_id=5)
         { scope, id=6 }
            [syscall] (SYSCALL_TOKEN, v_id=-1, s_id=0)
               [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [ref] (REF_TYPE_TOKEN, v_id=-1, s_id=0)
                  [c] (I8_VARIABLE_TOKEN, v_id=2, s_id=5)
               [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [_putn] (FUNC_NAME_TOKEN, v_id=2, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=7 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [a] (I32_VARIABLE_TOKEN, v_id=3, s_id=7)
         { scope, id=8 }
            [while] (WHILE_TOKEN, v_id=0, s_id=8)
               [>] (LARGER_TOKEN, v_id=-1, s_id=0)
                  [a] (I32_VARIABLE_TOKEN, v_id=3, s_id=7)
                  [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               { scope, id=9 }
                  [_putc] (CALL_TOKEN, v_id=1, s_id=0)
                     [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                        [%] (MODULO_TOKEN, v_id=-1, s_id=0)
                           [a] (I32_VARIABLE_TOKEN, v_id=3, s_id=7)
                           [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                        [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [/=] (DIVASSIGN_TOKEN, v_id=-1, s_id=0)
                     [a] (I32_VARIABLE_TOKEN, v_id=3, s_id=7)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [_puts] (FUNC_NAME_TOKEN, v_id=3, s_id=0)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=10 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [s] (I8_VARIABLE_TOKEN, ptr, v_id=4, s_id=10)
         { scope, id=11 }
            [while] (WHILE_TOKEN, v_id=0, s_id=11)
               [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                  [s] (I8_VARIABLE_TOKEN, ptr, v_id=4, s_id=10)
               { scope, id=12 }
                  [_putc] (CALL_TOKEN, v_id=1, s_id=0)
                     [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                        [s] (I8_VARIABLE_TOKEN, ptr, v_id=4, s_id=10)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                     [s] (I8_VARIABLE_TOKEN, ptr, v_id=4, s_id=10)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [printf] (FUNC_NAME_TOKEN, v_id=4, s_id=0)
      { scope, id=13 }
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=5, s_id=13)
         [...] (VAR_ARGUMENTS_TOKEN, v_id=-1, s_id=0)
         { scope, id=14 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [index] (I32_VARIABLE_TOKEN, v_id=6, s_id=14)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [i64] (I64_TYPE_TOKEN, v_id=-1, s_id=0)
               [fmt_len] (I64_VARIABLE_TOKEN, v_id=7, s_id=14)
               [_strlen] (CALL_TOKEN, v_id=0, s_id=0)
                  [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=5, s_id=13)
            [while] (WHILE_TOKEN, v_id=0, s_id=14)
               [<] (LOWER_TOKEN, v_id=-1, s_id=0)
                  [index] (I32_VARIABLE_TOKEN, v_id=6, s_id=14)
                  [fmt_len] (I64_VARIABLE_TOKEN, v_id=7, s_id=14)
               { scope, id=15 }
                  [switch] (SWITCH_TOKEN, v_id=0, s_id=15)
                     [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=5, s_id=13)
                        [index] (I32_VARIABLE_TOKEN, v_id=6, s_id=14)
                     { scope, id=0 }
                        [case] (CASE_TOKEN, v_id=-1, s_id=0)
                           [37] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                           { scope, id=16 }
                              [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
                                 [mode] (I8_VARIABLE_TOKEN, v_id=8, s_id=16)
                                 [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=5, s_id=13)
                                    [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                                       [index] (I32_VARIABLE_TOKEN, v_id=6, s_id=14)
                                       [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                              [switch] (SWITCH_TOKEN, v_id=0, s_id=16)
                                 [mode] (I8_VARIABLE_TOKEN, v_id=8, s_id=16)
                                 { scope, id=0 }
                                    [case] (CASE_TOKEN, v_id=-1, s_id=0)
                                       [115] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                                       { scope, id=17 }
                                          [_puts] (CALL_TOKEN, v_id=3, s_id=0)
                                             [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                                                [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
                                                [poparg] (, v_id=-1, s_id=0)
                                          [break] (BREAK_TOKEN, v_id=-1, s_id=0)
                                    [case] (CASE_TOKEN, v_id=-1, s_id=0)
                                       [117] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                                       { scope, id=18 }
                                    [case] (CASE_TOKEN, v_id=-1, s_id=0)
                                       [105] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                                       { scope, id=19 }
                                    [case] (CASE_TOKEN, v_id=-1, s_id=0)
                                       [108] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                                       { scope, id=20 }
                                    [case] (CASE_TOKEN, v_id=-1, s_id=0)
                                       [100] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                                       { scope, id=21 }
                                          [_putn] (CALL_TOKEN, v_id=2, s_id=0)
                                             [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                                                [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
                                                [poparg] (, v_id=-1, s_id=0)
                                          [break] (BREAK_TOKEN, v_id=-1, s_id=0)
                              [break] (BREAK_TOKEN, v_id=-1, s_id=0)
                        [default] (DEFAULT_TOKEN, v_id=-1, s_id=0)
                           { scope, id=22 }
                              [_putc] (CALL_TOKEN, v_id=1, s_id=0)
                                 [fmt] (I8_VARIABLE_TOKEN, ptr, v_id=5, s_id=13)
                                    [index] (I32_VARIABLE_TOKEN, v_id=6, s_id=14)
                              [break] (BREAK_TOKEN, v_id=-1, s_id=0)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                     [index] (I32_VARIABLE_TOKEN, v_id=6, s_id=14)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [start] (START_TOKEN, v_id=5, s_id=0)
      { scope, id=23 }
         [printf] (CALL_TOKEN, v_id=4, s_id=0)
            [Hello %s! Im %i yo!
] (STRING_VALUE_TOKEN, v_id=0, s_id=0)
            [Max] (STRING_VALUE_TOKEN, v_id=1, s_id=0)
            [22] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
         [exit] (EXIT_TOKEN, v_id=0, s_id=23)
            [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
:
function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    if not num; {
        dref buffer = '0';
        return;
    }

    i32 i = 0;
    arr tmp[32, i8];
    while num > 0 as i32; {
        tmp[i] = '0' + (num % 10) as i8;
        num /= 10;
        i += 1;
    }

    i32 j = 0;
    while i > 0 && j < blen; {
        buffer[j] = tmp[i];
        j += 1;
        i -= 1;
    }
}

glob arr buffer[32, i8] = { 0 };

start() {
    itoa(123321, ref buffer, sizeof(buffer) as i32);
    syscall(0x2000004, 1, ref buffer, sizeof(buffer));
}

:/ OUTPUT
{ scope, id=1 }
   [function] (FUNC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [itoa] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=1, s_id=-1)
            [num] (I32_VARIABLE_TOKEN, v_id=0, t_id=-1, s_id=2)
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, t_id=3, s_id=-1)
            [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=1, t_id=-1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=4, s_id=-1)
            [blen] (I32_VARIABLE_TOKEN, v_id=2, t_id=0, s_id=2)
         { scope, id=3 }
            [if] (IF_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [not] (NOT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [num] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
               { scope, id=4 }
                  [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [dref] (DREF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=1, t_id=3, s_id=2)
                     [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [return] (RETURN_TOKEN, v_id=-1, t_id=-1, s_id=4)
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=5, s_id=-1)
               [i] (I32_VARIABLE_TOKEN, v_id=3, t_id=0, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [arr] (ARRAY_TYPE_TOKEN, v_id=-1, t_id=7, s_id=-1)
               [tmp] (ARR_VARIABLE_TOKEN, v_id=4, t_id=7, s_id=3)
               [32] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=6, s_id=-1)
            [while] (WHILE_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [>] (LARGER_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [num] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                  [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               { scope, id=5 }
                  [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [[]]
                        [tmp] (ARR_VARIABLE_TOKEN, v_id=4, t_id=7, s_id=3)
                        [i] (I32_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=3)
                     [+] (PLUS_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                        [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                           [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=6, s_id=-1)
                           [%] (MODULO_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                              [num] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                              [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [/=] (DIVASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [num] (I32_VARIABLE_TOKEN, v_id=0, t_id=1, s_id=2)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [i] (I32_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=8, s_id=-1)
               [j] (I32_VARIABLE_TOKEN, v_id=5, t_id=0, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [while] (WHILE_TOKEN, v_id=-1, t_id=-1, s_id=3)
               [&&] (AND_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [>] (LARGER_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [i] (I32_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=3)
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [<] (LOWER_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [j] (I32_VARIABLE_TOKEN, v_id=5, t_id=8, s_id=3)
                     [blen] (I32_VARIABLE_TOKEN, v_id=2, t_id=4, s_id=2)
               { scope, id=6 }
                  [=] (ASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [[]]
                        [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=1, t_id=3, s_id=2)
                        [j] (I32_VARIABLE_TOKEN, v_id=5, t_id=8, s_id=3)
                     [[]]
                        [tmp] (ARR_VARIABLE_TOKEN, v_id=4, t_id=7, s_id=3)
                        [i] (I32_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=3)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [j] (I32_VARIABLE_TOKEN, v_id=5, t_id=8, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [-=] (SUBASSIGN_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [i] (I32_VARIABLE_TOKEN, v_id=3, t_id=5, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [arr] (ARRAY_TYPE_TOKEN, v_id=-1, t_id=9, s_id=-1, glob)
      [buffer] (ARR_VARIABLE_TOKEN, v_id=6, t_id=9, s_id=1, glob)
      [32] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
      [i8] (I8_TYPE_TOKEN, v_id=-1, t_id=6, s_id=-1)
      [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
   [start] (START_TOKEN, v_id=1, t_id=-1, s_id=1)
      { scope, id=7 }
         [()]
            [itoa] (FUNC_NAME_TOKEN, v_id=0, t_id=-1, s_id=1)
            { scope, id=-1 }
               [123321] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [buffer] (ARR_VARIABLE_TOKEN, v_id=6, t_id=9, s_id=1, glob)
               [as] (CONVERT_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                  [i32] (I32_TYPE_TOKEN, v_id=-1, t_id=0, s_id=-1)
                  [sizeof] (SIZEOF_TOKEN, v_id=-1, t_id=-1, s_id=-1)
                     [buffer] (ARR_VARIABLE_TOKEN, v_id=6, t_id=-1, s_id=1, glob)
         [syscall] (SYSCALL_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, t_id=-1, s_id=-1)
            [ref] (REF_TYPE_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [buffer] (ARR_VARIABLE_TOKEN, v_id=6, t_id=9, s_id=1, glob)
            [sizeof] (SIZEOF_TOKEN, v_id=-1, t_id=-1, s_id=-1)
               [buffer] (ARR_VARIABLE_TOKEN, v_id=6, t_id=-1, s_id=1, glob)
/:
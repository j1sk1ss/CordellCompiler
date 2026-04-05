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
   [function] (FUNC_TOKEN, v_id=-1, s_id=0)
      [itoa] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         [i0] (I0_TYPE_TOKEN, v_id=-1, s_id=0)
      { scope, id=2 }
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [num] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
         [i8] (I8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
            [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=1, s_id=2)
         [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
            [blen] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
         { scope, id=3 }
            [if] (IF_TOKEN, v_id=-1, s_id=3)
               [not] (NEGATIVE_TOKEN, v_id=-1, s_id=0)
                  [num] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
               { scope, id=4 }
                  [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
                     [dref] (DREF_TYPE_TOKEN, v_id=-1, s_id=0)
                        [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=1, s_id=2)
                     [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [return] (RETURN_TOKEN, v_id=-1, s_id=4)
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [i] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [arr] (ARRAY_TYPE_TOKEN, v_id=-1, s_id=0)
               [tmp] (ARR_VARIABLE_TOKEN, v_id=4, s_id=3)
               [32] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
            [while] (WHILE_TOKEN, v_id=-1, s_id=3)
               [>] (LARGER_TOKEN, v_id=-1, s_id=0)
                  [num] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
                  [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                     [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               { scope, id=5 }
                  [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
                     [[]]
                        [tmp] (ARR_VARIABLE_TOKEN, v_id=4, s_id=3)
                        [i] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
                     [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                        [48] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                        [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                           [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
                           [%] (MODULO_TOKEN, v_id=-1, s_id=0)
                              [num] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
                              [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [/=] (DIVASSIGN_TOKEN, v_id=-1, s_id=0)
                     [num] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
                     [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                     [i] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [j] (I32_VARIABLE_TOKEN, v_id=5, s_id=3)
               [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [while] (WHILE_TOKEN, v_id=-1, s_id=3)
               [&&] (AND_TOKEN, v_id=-1, s_id=0)
                  [>] (LARGER_TOKEN, v_id=-1, s_id=0)
                     [i] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
                     [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [<] (LOWER_TOKEN, v_id=-1, s_id=0)
                     [j] (I32_VARIABLE_TOKEN, v_id=5, s_id=3)
                     [blen] (I32_VARIABLE_TOKEN, v_id=2, s_id=2)
               { scope, id=6 }
                  [=] (ASSIGN_TOKEN, v_id=-1, s_id=0)
                     [[]]
                        [buffer] (I8_VARIABLE_TOKEN, ptr, v_id=1, s_id=2)
                        [j] (I32_VARIABLE_TOKEN, v_id=5, s_id=3)
                     [[]]
                        [tmp] (ARR_VARIABLE_TOKEN, v_id=4, s_id=3)
                        [i] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
                  [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                     [j] (I32_VARIABLE_TOKEN, v_id=5, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
                  [-=] (SUBASSIGN_TOKEN, v_id=-1, s_id=0)
                     [i] (I32_VARIABLE_TOKEN, v_id=3, s_id=3)
                     [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [arr] (ARRAY_TYPE_TOKEN, v_id=-1, s_id=0, glob)
      [buffer] (ARR_VARIABLE_TOKEN, v_id=6, s_id=1, glob)
      [32] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
      [i8] (I8_TYPE_TOKEN, v_id=-1, s_id=0)
      [0] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
   [start] (START_TOKEN, v_id=1, s_id=1)
      { scope, id=7 }
         [itoa] (CALL_TOKEN, v_id=0, s_id=0)
            { scope, id=0 }
               [123321] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
               [ref] (REF_TYPE_TOKEN, v_id=-1, s_id=0)
                  [buffer] (ARR_VARIABLE_TOKEN, v_id=6, s_id=1, glob)
               [as] (CONVERT_TOKEN, v_id=-1, s_id=0)
                  [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
                  [sizeof] (SIZEOF_TOKEN, v_id=-1, s_id=0)
                     [buffer] (ARR_VARIABLE_TOKEN, v_id=6, s_id=1, glob)
         [syscall] (SYSCALL_TOKEN, v_id=-1, s_id=0)
            [33554436] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0)
            [ref] (REF_TYPE_TOKEN, v_id=-1, s_id=0)
               [buffer] (ARR_VARIABLE_TOKEN, v_id=6, s_id=1, glob)
            [sizeof] (SIZEOF_TOKEN, v_id=-1, s_id=0)
               [buffer] (ARR_VARIABLE_TOKEN, v_id=6, s_id=1, glob)
/:
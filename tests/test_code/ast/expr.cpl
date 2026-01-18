{
    start() {
        i32 a = 10 + 11 * 12;
        i32 b = 10 - 1;
        a += b += a; 
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=0 }
         { scope, id=2 }
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
               [+] (PLUS_TOKEN, v_id=-1, s_id=0)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0, glob)
                  [*] (MULTIPLY_TOKEN, v_id=-1, s_id=0)
                     [11] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0, glob)
                     [12] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0, glob)
            [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0)
               [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
               [-] (MINUS_TOKEN, v_id=-1, s_id=0)
                  [10] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0, glob)
                  [1] (UNKNOWN_NUMERIC_TOKEN, v_id=-1, s_id=0, glob)
            [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
               [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
               [+=] (ADDASSIGN_TOKEN, v_id=-1, s_id=0)
                  [b] (I32_VARIABLE_TOKEN, v_id=1, s_id=2)
                  [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=2)
:

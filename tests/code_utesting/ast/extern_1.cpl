{
    extern i32 a;
    extern function printf(ptr u8 fmt, ...);
}

: OUTPUT
{ scope, id=1 }
   [extern] (EXTERN_TOKEN, v_id=-1, s_id=0)
      [i32] (I32_TYPE_TOKEN, v_id=-1, s_id=0, ext)
         [a] (I32_VARIABLE_TOKEN, v_id=0, s_id=1)
   [extern] (EXTERN_TOKEN, v_id=-1, s_id=0)
      [function] (FUNC_PROT_TOKEN, v_id=-1, s_id=0)
         [printf] (FUNC_NAME_TOKEN, v_id=0, s_id=1)
         { scope, id=2 }
            [u8] (U8_TYPE_TOKEN, ptr, v_id=-1, s_id=0)
               [fmt] (U8_VARIABLE_TOKEN, ptr, v_id=1, s_id=2)
            [...] (VAR_ARGUMENTS_TOKEN, v_id=-1, s_id=0)
:
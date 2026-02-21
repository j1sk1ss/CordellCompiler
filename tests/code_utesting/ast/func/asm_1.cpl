{
    start() {
        asm () {
            "mov rdx, 60",
            "syscall"
        }
    }
}

: OUTPUT
{ scope, id=1 }
   [start] (START_TOKEN, v_id=0, s_id=0)
      { scope, id=2 }
         [asm] (ASM_TOKEN, v_id=-1, s_id=0)
            { scope, id=0 }
            { scope, id=0 }
               [mov rdx, 60] (STRING_VALUE_TOKEN, v_id=0, s_id=0)
               [syscall] (STRING_VALUE_TOKEN, v_id=1, s_id=0)
:
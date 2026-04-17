: BLOCK_TEST :
: RUN_ASM[args="0"|args="1"] :
@[entry("_main")]
function main(i32 argc, ptr ptr i8 argv) -> i0 {
    if argv[1][0] == '0'; exit 3;
    else                  exit 7;
}

:/ OUTPUT
@case_index=0
@exit_code=3
---
@case_index=1
@exit_code=7
---
/:

#ifndef LIR_TYPES_H_
#define LIR_TYPES_H_

typedef enum {
    /* Operations */
        /* Commands */
        LIR_FCLL,  // function call
        LIR_ECLL,  // extern function call
        LIR_STRT,  // start macro
        LIR_STEND, // end macro
        LIR_SYSC,  // syscall
        LIR_FRET,  // function ret
        LIR_TST,   // test
        LIR_XCHG,  // xchg
        LIR_CDQ,   // cdq
        LIR_MKLB,  // mk label
        LIR_FDCL,  // declare function
        LIR_FEND,
        LIR_OEXT,  // extern object

        LIR_CMP,

        LIR_SETL,
        LIR_SETG,
        LIR_STLE,
        LIR_STGE,
        LIR_SETE,
        LIR_STNE,
        LIR_SETB,
        LIR_SETA,
        LIR_STBE,
        LIR_STAE,

        /* Jump instructions */
        LIR_JMP,  // jmp
        LIR_JL,   // jump if less (signed)
        LIR_JG,   // jump if greater (signed)
        LIR_JLE,  // jump if less or equal (signed)
        LIR_JGE,  // jump if greater or equal (signed)
        LIR_JE,   // jump if equal / zero
        LIR_JNE,  // jump if not equal / not zero
        LIR_JB,   // jump if below (unsigned <)
        LIR_JA,   // jump if above (unsigned >)
        LIR_JBE,  // jump if below or equal (unsigned <=)
        LIR_JAE,  // jump if above or equal (unsigned >=)

        /* Data commands */
        LIR_RESV,
        LIR_VDCL,

    /* Register */
        /* Operations */
        LIR_iMOV,  // integer move, x = y

        LIR_STARGLD, // st load
        LIR_STARGRF, // st ref load

        LIR_VRDEALL, // cnst_x=var_id
        LIR_STRDECL, // cnst_x=var_id, cnst_y=str_id
        LIR_ARRDECL, // cnst_x=var_id

        LIR_STSARG,   // store parameter to syscall, x, cnst_y=index
        LIR_STFARG,   // store parameter to function, x, cnst_y=index
        LIR_LOADFARG, // load parameter in function, cnst_x=var_id, cnst_y=index
        LIR_LOADFRET, // load funcret to dst, cnst_x=var_id

        LIR_TF64,     // x = (f64)y
        LIR_TF32,     // x = (f32)y
        LIR_TI64,     // x = (i64)y
        LIR_TI32,     // x = (i32)y
        LIR_TI16,     // x = (i16)y
        LIR_TI8,      // x = (i8)y
        LIR_TU64,     // x = (u64)y
        LIR_TU32,     // x = (u32)y
        LIR_TU16,     // x = (u16)y
        LIR_TU8,      // x = (u8)y

        LIR_CVTTSS2SI,
        LIR_CVTTSD2SI,
        LIR_CVTSI2SS,
        LIR_CVTSI2SD,
        LIR_CVTSS2SD,
        LIR_CVTSD2SS,
        LIR_MOVSX,
        LIR_MOVZX,
        LIR_MOVSXD,

        LIR_NOT,

        LIR_fMOV,  // float move
        LIR_fMVf,  // float to float move
        LIR_REF,   // lea move
        LIR_GDREF, // get value from address, x = *y
        LIR_LDREF, // set valye by address,   *x = y
        LIR_PUSH,  // push
        LIR_POP,   // pop

    /* Integer */
        /* Binary operations */
        LIR_iADD, // x = y + z
        LIR_iSUB, // x = y - z
        LIR_iMUL, // x = y * z
        LIR_DIV,  // x = y / z
        LIR_iDIV, // x = y / z
        LIR_iMOD, // x = y % z
        LIR_iLRG, // x = y > z
        LIR_iLGE, // x = y >= z
        LIR_iLWR, // x = y < z
        LIR_iLRE, // x = y <= z
        LIR_iCMP, // x = y == z
        LIR_iNMP, // x = y != z

        /* Logic */
        LIR_iAND, // x = y && z
        LIR_iOR,  // x = y || z

    /* Float */
        /* Binary operations */
        LIR_fADD, // x = y f+ z
        LIR_fSUB, // x = y f- z
        LIR_fMUL, // x = y f* z
        LIR_fDIV, // x = y f/ z
        LIR_fCMP, // x = y f== z

    /* Bits */
        /* Binary operations */
        LIR_iBLFT, // bit left
        LIR_iBRHT, // bit right
        LIR_bAND, // bit and
        LIR_bOR,  // bit or
        LIR_bXOR, // bit xor
        LIR_bSHL, // bitleft
        LIR_bSHR, // bitright
        LIR_bSAR, // bitleft unsgn

    /* Other */
    LIR_RAW,
    LIR_BREAKPOINT,

    /* High level operations */
        /* Stack */
        LIR_RSVSTK, // Reserve stackframe
        LIR_MKSCOPE,
        LIR_ENDSCOPE,
    
        /* Heap */
        LIR_ALLCH,  // allocate heap + save addr to farg
        LIR_DEALLH, // deallocate heap, addr from farg

        /* System */
        LIR_EXITOP, // Exit with farg exit call
        LIR_VRUSE,
        LIR_BB,
} lir_operation_t;

typedef enum {
    LIR_REGISTER,
    LIR_STVARIABLE,
    LIR_VARIABLE,
    LIR_GLVARIABLE,
    LIR_CONSTVAL,
    LIR_NUMBER,
    LIR_LABEL,
    LIR_RAWASM,
    LIR_MEMORY,
    LIR_FNAME,
    LIR_STRING,
    LIR_ARGLIST,
} lir_subject_type_t;

typedef enum {
    RAX,  RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8,  R9,  R10,  R11,  R12,  R13,  R14,  R15,
    EAX,  EBX, ECX, EDX, ESI, EDI, EBP, ESP, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D,
    AX,   BX,  CX,  DX,  SI,  DI,  BP,  SP,  R8W, R9W, R10W, R11W, R12W, R13W, R14W, R15W,
    AL,   BL,  CL,  DL,  SIL, DIL, BPL, SPL, R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B,
    AH,   BH,  CH,  DH, 
    XMM0, XMM1, XMM2, XMM3, XMM4, 
} lir_registers_t;
#define FREE_REGISTERS      4
#define FIRST_FREE_REGISTER R12

lir_registers_t LIR_format_register(lir_registers_t reg, int size);
int LIR_movop(lir_operation_t op);
int LIR_writeop(lir_operation_t op);
int LIR_readop(lir_operation_t op);

#endif
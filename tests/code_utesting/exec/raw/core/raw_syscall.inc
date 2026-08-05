#ifndef RAW_CORE_SYSCALL_INC_
#define RAW_CORE_SYSCALL_INC_ 0

#ifdef CCPL_MACHO64
    #define RAW_SYS_WRITE 0x2000004
#endif

#ifdef CCPL_GNU64
    #define RAW_SYS_WRITE 1
#endif

#ifdef CCPL_GNUI386
    #define RAW_SYS_WRITE 4
#endif

#endif

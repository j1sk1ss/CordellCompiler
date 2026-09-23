#ifndef SYSCALL_H_
#define SYSCALL_H_

#include <hir/hir.h>

typedef struct {
    hir_subject_type_t t;           /* HIR type               */
    int                ptr;         /* Reference level        */
    int                dereference; /* Will deredference?     */
    const char*        name;        /* Argument's name        */
    const char*        description; /* Argument's description */
} expected_type_t;

typedef struct {
    unsigned long   number;         /* Syscall number                   */
    int             argc;           /* Arguments count                  */
    expected_type_t types[10];      /* Arguments' types                 */
    expected_type_t rtype;          /* Return's type                    */
    const char*     name;           /* Syscall name                     */
    const char*     description;    /* Syscall description              */
    int             security;       /* Levels are the next:
                                    1 - read-only queries and ordinary reads;
                                    2 - local writes or local process/filesystem state changes;
                                    3 - sockets, IPC, and cross-process interaction;
                                    4 - privileged, execution, kernel, or system-wide control operations. */
} syscall_t;

#define ET(_type, _ptr, _deref, _name, _descr) \
    ((expected_type_t){ .t = (_type), .ptr = (_ptr), .dereference = (_deref), .name = (_name), .description = (_descr) })
#define ARG(_type, _deref, _name, _descr) \
    ET((_type), 0, (_deref), (_name), (_descr))
#define PTR(_type, _deref, _name, _descr) \
    ET((_type), 1, (_deref), (_name), (_descr))
#define PTR2(_type, _deref, _name, _descr) \
    ET((_type), 2, (_deref), (_name), (_descr))
#define SYSCALL0(num, ret, sname, sdescr, sec) \
    { .number = (num), .argc = 0, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL1(num, ret, sname, sdescr, sec, a1) \
    { .number = (num), .argc = 1, .types = { (a1) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL2(num, ret, sname, sdescr, sec, a1, a2) \
    { .number = (num), .argc = 2, .types = { (a1), (a2) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL3(num, ret, sname, sdescr, sec, a1, a2, a3) \
    { .number = (num), .argc = 3, .types = { (a1), (a2), (a3) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL4(num, ret, sname, sdescr, sec, a1, a2, a3, a4) \
    { .number = (num), .argc = 4, .types = { (a1), (a2), (a3), (a4) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL5(num, ret, sname, sdescr, sec, a1, a2, a3, a4, a5) \
    { .number = (num), .argc = 5, .types = { (a1), (a2), (a3), (a4), (a5) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL6(num, ret, sname, sdescr, sec, a1, a2, a3, a4, a5, a6) \
    { .number = (num), .argc = 6, .types = { (a1), (a2), (a3), (a4), (a5), (a6) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL7(num, ret, sname, sdescr, sec, a1, a2, a3, a4, a5, a6, a7) \
    { .number = (num), .argc = 7, .types = { (a1), (a2), (a3), (a4), (a5), (a6), (a7) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }
#define SYSCALL8(num, ret, sname, sdescr, sec, a1, a2, a3, a4, a5, a6, a7, a8) \
    { .number = (num), .argc = 8, .types = { (a1), (a2), (a3), (a4), (a5), (a6), (a7), (a8) }, .rtype = (ret), .name = (sname), .description = (sdescr), .security = (sec) }

int SYSCHECK_get_macoh_x86_64_syscall_table(syscall_t** dest);
int SYSCHECK_get_linux_x86_64_syscall_table(syscall_t** dest);

#endif

#ifndef SYSCALL_H_
#define SYSCALL_H_

#include <hir/hir.h>

typedef struct {
    hir_subject_type_t t;
    int                ptr;
} expected_type_t;

typedef struct {
    unsigned long   number;
    int             argc;
    expected_type_t types[10];
    expected_type_t rtype;
    const char*     name;
} syscall_t;

#define ET(_type, _ptr) ((expected_type_t){ .t = (_type), .ptr = (_ptr) })
#define ARG(_type)      ET((_type), 0)
#define PTR(_type)      ET((_type), 1)
#define PTR2(_type)     ET((_type), 2)

#define SYSCALL0(num, ret) \
    { .number = (num), .argc = 0, .rtype = (ret), .name = #num }
#define SYSCALL1(num, ret, a1) \
    { .number = (num), .argc = 1, .types = { (a1) }, .rtype = (ret), .name = #num }
#define SYSCALL2(num, ret, a1, a2) \
    { .number = (num), .argc = 2, .types = { (a1), (a2) }, .rtype = (ret), .name = #num }
#define SYSCALL3(num, ret, a1, a2, a3) \
    { .number = (num), .argc = 3, .types = { (a1), (a2), (a3) }, .rtype = (ret), .name = #num }
#define SYSCALL4(num, ret, a1, a2, a3, a4) \
    { .number = (num), .argc = 4, .types = { (a1), (a2), (a3), (a4) }, .rtype = (ret), .name = #num }
#define SYSCALL5(num, ret, a1, a2, a3, a4, a5) \
    { .number = (num), .argc = 5, .types = { (a1), (a2), (a3), (a4), (a5) }, .rtype = (ret), .name = #num }
#define SYSCALL6(num, ret, a1, a2, a3, a4, a5, a6) \
    { .number = (num), .argc = 6, .types = { (a1), (a2), (a3), (a4), (a5), (a6) }, .rtype = (ret), .name = #num }
#define SYSCALL7(num, ret, a1, a2, a3, a4, a5, a6, a7) \
    { .number = (num), .argc = 6, .types = { (a1), (a2), (a3), (a4), (a5), (a6), (a7) }, .rtype = (ret), .name = #num }
#define SYSCALL8(num, ret, a1, a2, a3, a4, a5, a6, a7, a8) \
    { .number = (num), .argc = 6, .types = { (a1), (a2), (a3), (a4), (a5), (a6), (a7), (a8) }, .rtype = (ret), .name = #num }

int SYSCHECK_get_macos_syscall_table(syscall_t** dest);

#endif

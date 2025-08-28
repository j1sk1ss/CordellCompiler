# x86_64 GNU NASM microcode generator
This module generates NASM x86_64 GNU code. Let's look at some concrete examples. </br>
Note: This microcode example was generated without any optimization approaches.

## Local and global variable declaration
Source code:
```CPL
{
    glob int glint = 1;
    ro int glroint = 2;

    start {
        long ow = 1234354576;
        int a = 0;
        int b = 10;
        int c = (a + c) * a;
        
        {
            short dfg = 2;
        }

        char dfg = 2;
        exit 0;
    };
}
```

NASM microcode:
```ASM
section .data
__glint__ dd 1
section .rodata
__glroint__ dd 2
section .bss
section .text
mov qword [rbp - 8], 1234354576
mov dword [rbp - 8], 0
mov dword [rbp - 8], 10
mov eax, [rbp - 8]
push rax
mov eax, [rbp - 8]
pop rbx
add rax, rbx
push rax
mov eax, [rbp - 8]
pop rbx
imul rax, rbx
mov [rbp - 8], rax
mov word [rbp - 16], 2
mov byte [rbp - 8], 2
mov rax, 0
mov rdi, rax
mov rax, 60
syscall
```

## Array and string declaration
Source code:
```CPL
{
    glob str string = "String variable!";
    start {
        str hello_msg = "Hello there!";
        arr str_arr[12, char] = { 'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0 };
        
        {
            arr buff[2, char] = { 0, 0 };
        }
        
        arr buff[2, char] = { 0, 0 };
        buff[0] = 'A';
        char a = buff[0];
        ptr char b = buff;

        exit 0;
    }
}
```

NASM microcode:
```ASM
section .data
__string__ db 'str_0', 0
section .rodata
__str_0__ db 'String variable!', 0
section .bss
section .text
mov byte [rbp - 40], 72
mov byte [rbp - 39], 101
mov byte [rbp - 38], 108
mov byte [rbp - 37], 108
mov byte [rbp - 36], 111
mov byte [rbp - 35], 32
mov byte [rbp - 34], 116
mov byte [rbp - 33], 104
mov byte [rbp - 32], 101
mov byte [rbp - 31], 114
mov byte [rbp - 30], 101
mov byte [rbp - 29], 33
mov byte [rbp - 40], 72
mov byte [rbp - 39], 101
mov byte [rbp - 38], 108
mov byte [rbp - 37], 108
mov byte [rbp - 36], 111
mov byte [rbp - 35], 32
mov byte [rbp - 34], 119
mov byte [rbp - 33], 111
mov byte [rbp - 32], 114
mov byte [rbp - 31], 108
mov byte [rbp - 30], 100
mov byte [rbp - 29], 0
mov byte [rbp - 48], 0
mov byte [rbp - 47], 0
mov byte [rbp - 40], 0
mov byte [rbp - 39], 0
mov al, 65
mov rdx, rax
mov rax, 0
lea rbx, [rbp - 40]
add rax, rbx
mov byte ptr [rax], rdx
mov rax, 0
lea rbx, [rbp - 40]
add rax, rbx
mov rax, [rax]
mov [rbp - 40], rax
lea rax, [rbp - 40]
mov [rbp - 40], rax
mov rax, 0
mov rdi, rax
mov rax, 60
syscall
```

## Operand operation generation
Source code:
```CPL
{
    start {
        int a = 10 * 12 + (13 + 14) * 15;
        int b = 10 * ((12 + 13) + 14) * 15;
        int c = 10 * 12 + 13 + 14 * 15;
        int k = 10 / 12 + (13 || 14) * 15;
        int l = 10 % ((12 * 13) + 14) * 15;
        int m = 10 | 12 + 13 && 14 * 15;
        exit c;
    }
}
```

NASM microcode:
```ASM
section .data
section .rodata
section .bss
section .text
mov rax, 10
push rax
mov rax, 12
pop rbx
imul rax, rbx
push rax
mov rax, 13
push rax
mov rax, 14
pop rbx
add rax, rbx
push rax
mov rax, 15
pop rbx
imul rax, rbx
pop rbx
add rax, rbx
mov [rbp - 8], rax
mov rax, 10
push rax
mov rax, 12
push rax
mov rax, 13
pop rbx
add rax, rbx
push rax
mov rax, 14
pop rbx
add rax, rbx
pop rbx
imul rax, rbx
push rax
mov rax, 15
pop rbx
imul rax, rbx
mov [rbp - 8], rax
mov rax, 10
push rax
mov rax, 12
pop rbx
imul rax, rbx
push rax
mov rax, 13
pop rbx
add rax, rbx
push rax
mov rax, 14
push rax
mov rax, 15
pop rbx
imul rax, rbx
pop rbx
add rax, rbx
mov [rbp - 8], rax
mov rax, 10
push rax
mov rax, 12
mov rbx, rax
pop rax
cdq
idiv rbx
push rax
push rax
mov rax, 15
pop rbx
imul rax, rbx
pop rbx
add rax, rbx
mov [rbp - 8], rax
push rax
mov rax, 15
pop rbx
imul rax, rbx
mov [rbp - 8], rax
mov [rbp - 8], rax
mov eax, [rbp - 8]
mov rdi, rax
mov rax, 60
syscall
```

## Branch generation
Source code:
```CPL
{
    start {
        while 1; {
            exit 1;
        }
        else {
            int a = 10;
            exit a;
        }

        if 1; {
            exit 1;
        }
        else {
            int a = 10;
            exit a;
        }

        switch 1; {
            case 1; {
                exit 1;
            }
            default {
                int a = 10;
                exit a;
            }
        }
    }
}
```

NASM microcode:
```ASM
section .data
section .rodata
section .bss
section .text
__while_0__:
mov rax, 1
cmp rax, 0
je __end_while_0__
mov rax, 1
mov rdi, rax
mov rax, 60
syscall
mov dword [rbp - 8], 10
mov eax, [rbp - 8]
mov rdi, rax
mov rax, 60
syscall
jmp __while_0__
__end_while_0__:
mov dword [rbp - 8], 10
mov eax, [rbp - 8]
mov rdi, rax
mov rax, 60
syscall
mov rax, 1
cmp rax, 0
je __else_1__
mov rax, 1
mov rdi, rax
mov rax, 60
syscall
mov dword [rbp - 8], 10
mov eax, [rbp - 8]
mov rdi, rax
mov rax, 60
syscall
jmp __end_if_1__
__else_1__:
mov dword [rbp - 8], 10
mov eax, [rbp - 8]
mov rdi, rax
mov rax, 60
syscall
__end_if_1__:
jmp __end_cases_2__
__case_1_2__:
mov rax, 1
mov rdi, rax
mov rax, 60
syscall
jmp __end_switch_2__
__default_2__:
mov dword [rbp - 8], 10
mov eax, [rbp - 8]
mov rdi, rax
mov rax, 60
syscall
jmp __end_switch_2__
__end_cases_2__:
mov rax, 1
cmp rax, 1
jl __case_l_1_2__
jg __case_r_1_2__
jmp __case_1_2__
__case_l_1_2__:
jmp __default_2__
__case_r_1_2__:
jmp __default_2__
__end_switch_2__:
```

## strlen function generation
Source code:
```CPL
function strlen(ptr char string) {
    int length = 0;
    while string[0]; {
        length = length + 1;
        string = string + 1;
    }

    return length;
}
```

NASM microcode:
```ASM
global __strlen__
jmp __end_strlen__
__strlen__:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp - 8], rsi
    mov dword [rbp - 16], 0
    __while_0__:
        mov rax, 0
        lea rbx, [rbp - 8]
        add rax, rbx
        mov rax, [rax]
        cmp rax, 0
        je __end_while_0__
        mov eax, [rbp - 16]
        push rax
        mov rax, 1
        pop rbx
        add rax, rbx
        mov dword ptr [rbp - 16], eax
        mov rax, [rbp - 8]
        push rax
        mov rax, 1
        pop rbx
        add rax, rbx
        mov qword ptr [rbp - 8], rax
        jmp __while_0__
        __end_while_0__:
    mov eax, [rbp - 16]
    mov rsp, rbp
    pop rbp
    ret
__end_strlen__:
```

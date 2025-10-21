# Codegen (nasm) part
After completing the full code transformation pipeline, we can safely convert our `LIR` form into the `ASM` form, with a few small tricks applied during the unwrap process of special `LIR` instructions such as `EXITOP`, `STRT`, and others.
![lir2asm](../../media/LIR_to_ASM.png)

## Example of generated code
```
section .data
section .rodata
section .bss
section .text
global _cpl_start
_cpl_sum:
push rbp
mov rbp, rsp
sub rsp, 8
mov r13d, edi
mov r12d, esi
mov [rbp - 8], r12d
mov [rbp - 4], r13d
mov rbx, 0
imul rbx, 4
lea rax, [rbp - 8]
add rax, rbx
mov rax, [rax]
mov r11d, eax
mov rbx, 1
imul rbx, 4
lea rax, [rbp - 8]
add rax, rbx
mov rax, [rax]
mov r11d, eax
mov rax, r11d
mov rbx, r11d
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rsp, rbp
pop rbp
ret
; op=14
global _start
_start:
push rbp
mov rbp, rsp
sub rsp, 32
mov rax, [rbp + 8]
mov r13, rax
lea rax, [rbp + 16]
mov r13, rax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 8], eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 16], eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 24], eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov r15d, eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov r14d, eax
mov rax, 10
mov eax, rax
mov r11d, eax
mov rax, r11d
mov [rbp - 32], eax
push r11
push r12
push r13
push r14
push r15
mov rdi, [rbp - 16]
mov rsi, [rbp - 8]
call _cpl_[rbp - 8]
mov r13d, eax
pop r15
pop r14
pop r13
pop r12
pop r11
mov rax, [rbp - 8]
mov rbx, [rbp - 16]
imul rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, [rbp - 24]
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, r15d
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, r14d
add rax, rbx
mov r11d, eax
mov rax, r11d
mov rbx, [rbp - 32]
add rax, rbx
mov r11d, eax
mov rax, r13d
mov rbx, r11d
cmp rax, rbx
movzx rax, al
mov r11d, eax
cmp r11d, 0
jne lb73
mov rax, 60
mov rdi, 1
syscall
lb73:
lea rax, [rbp - 32]
mov r12, rax
mov rax, r12
mov eax, rax
mov r12d, eax
mov rax, r12d
mov r11d, eax
mov rax, 60
mov rdi, r11d
syscall
; op=4
```
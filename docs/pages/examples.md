# Examples
## strlen
```cpl
{
    function strlen(ptr i8 s) -> i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }
}
```

<details>
<summary><strong>More examples</strong></summary>

## print.cpl
```cpl
: strlen_h.cpl :
{
#ifndef STRLEN_H_
#define STRLEN_H_ 0
    function strlen(ptr i8 s) -> i64;
#endif
}
: print_h.cpl :
{
#ifndef PRINT_H_
#define PRINT_H_ 0
    function print(ptr i8 s) -> i0;
#endif
}
: print.cpl :
{
    #include <print_h.cpl>

    function print(ptr i8 s) -> i0 {
        syscall(1, 1, strlen(s), s);
    }
}
```

## memset.cpl
```cpl
: memset_h.cpl :
{
#ifndef MEMSET_H_
#define MEMSET_H_ 0
    function memset(ptr u8 buffer, u8 val, u64 size) -> i0;
#endif
}
: memset.cpl :
{
    #include <memset_h.cpl>

    function memset(ptr u8 buffer, u8 val, u64 size) -> i0 {
        u64 index = 0;
        while index < size; {
            buffer[index] = val;
            index += 1;
        }
    }
}
```

## file.cpl
```cpl
{
    function puts(ptr i8 s) -> i64 {
        return syscall(1, 1, s, strlen(s));
    }

    function putc(i8 c) -> i64 {
        return syscall(1, 1, ref c, 1);
    }

    function gets(ptr i8 buffer, i64 size) -> i64 {
        return syscall(0, 0, buffer, size);
    }

    function open(ptr i8 path, i32 flags, i32 mode) -> i64 {
        return syscall(2, path, flags, mode);
    }

    function fwrite(i32 fd, ptr u8 buffer, i32 size) -> i64 {
        return syscall(1, fd, buffer, size);
    }

    function fread(i32 fd, ptr u8 buffer, i32 size) -> i64 {
        return syscall(0, fd, buffer, size);
    }

    function close(i32 fd) -> i64 {
        return syscall(3, fd);
    }
}
```

## brainfuck.cpl
```cpl
{
    #include "stdio_h.cpl"

    glob arr tape[30000, i8];
    glob arr code[10000, i8];
    glob arr bracketmap[10000, i32];
    glob arr stack[10000, i32];

    @[entry]
    function brainfuck() {
        puts("Brainfuck interpriter! Input code: ");

        i32 pos = 0;
        i32 stackptr = 0;
        i32 codelength = gets(code, 10000);
        while pos < codelength; {
            @[no_fall]
            @[straight]
            switch code[pos]; {
                case '['; {
                    stack[stackptr] = pos;
                    stackptr += 1;
                }
                case ']'; {
                    if stackptr > 0; {
                        stackptr -= 1;
                        i32 matchpos = stack[stackptr];
                        bracketmap[pos] = matchpos;
                        bracketmap[matchpos] = pos;
                    }
                }
            }
            
            pos += 1;
        }
        
        i32 pc = 0;
        i32 pointer = 0;
        while pc < codelength; {
            @[no_fall] 
            switch code[pc]; {
                case '>'; {
                    pointer += 1;
                    pc += 1;
                }
                case '<'; {
                    pointer -= 1;
                    pc += 1;
                }
                case '+'; {
                    tape[pointer] += 1;
                    pc += 1;
                }
                case '-'; {
                    tape[pointer] -= 1;
                    pc += 1;
                }
                case '.'; {
                    putc(tape[pointer]);
                    pc += 1;
                }
                case ','; {
                    gets(ref tape[pointer], 1);
                    pc += 1;
                }
                case '['; {
                    if not tape[pointer]; pc = bracketmap[pc];
                    else pc += 1;
                }
                case ']'; {
                    if tape[pointer]; pc = bracketmap[pc];
                    else pc += 1;
                }
                @[cold] default {
                    pc += 1;
                }
            }
        }

        exit 0;
    }
}
```
</details>

# CPL code performance evaluation
**Important note:** some microbenchmarks use `asm volatile` in the C version to prevent dead-code elimination of otherwise empty or non-observable loops. CPL currently preserves such loops by default, while optimizing C compilers may remove them. These tests should therefore be interpreted as loop-overhead measurements rather than general-purpose performance benchmarks. </br>

Versions:
- `gcc-14`: GCC 14.2.0 (Homebrew GCC 14.2.0)
  - Optimizations: `-O0`, `-O3`
- `clang`: Apple clang 12.0.0 (clang-1200.0.32.29)
  - Optimizations: `-O0`, `-O3`
- `cpl`: CPL v3.4 (MACHO64)
  - Optimizations: `-O0`, `-O3` (LICM, Peephole, CSE, DCE, Constant prop / fold) 

Specs:
- OS: MacOS Catalina 10.15.7
- CPU: i7 3650QM, Quad-core, 2.4 GHz
- RAM: 16 GB, DDR3, 1600 MHz

Result gathering:
  - CPL - 5 times, every time after compilation, `py-time` total program execution time
  - Clang / GCC - 5 times, every time after compilation, `gnu-time` total execution time

*P.S.:* The results below can be considered as a valid value, but I'd suggest to add about `+30%` of consumed execution time for every CPL's result given possible issues in my measurement methods, hardware and software. Furthermore, such a suggestion is based on a `+%30` size difference between GCC's and Clang's `.asm` files with CPL's `.asm` files.

## Empty loop
This is an artificial example and it doesn't provide any real information about CPL as a compiler. But this type of tests shows that the produced by CPL assembly code is neither overwhelmed nor slow. In summary, these two snippets of code are the same, considering the fact that the CPL's snippet does include a 'hidden' variable which iterates 1 billion times. Meanwhile, the C's snippet also does include the `asm volatile` section which preserves the loop from elemination by an optimizing module.

```cpl
@[naked] start() {
    @[counter(1000000000)] loop {
    }
    exit 0;
}

:/ C version:
int main() {
    unsigned a = 0;
    for (; a < 1000000000u; ++a) {
        asm volatile("" : "+r"(a));
    }
    return 0;
}
/:
```

The results below shows that the optimized CPL code has the same execution time as it have both the GCC's code and CLang's code.

<div
  class="benchmark-card"
  data-title="Empty loop benchmark"
  data-labels="cpl -O3|clang -O3|gcc-14 -O3|cpl -O0|gcc-14 -O0|clang -O0"
  data-values="0.722|0.748|0.758|1.52|4.290|4.641"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 18 lines (including comments such as a base block number). Were 31 lines.

## Million fibonacci
```cpl
start() {
    i32 a = 1;
    i32 b = 0;
    @[counter(1000000)] loop {
        i32 tmp = a;
        a = a + b;
        b = tmp;
    }
    exit b;
}

:/ C version
int main() {
    int a = 1;
    int b = 0;
    for (int i = 0; i < 1000000u; ++i) {
        int tmp = a;
        a = a + b;
        b = tmp;
    }
    return b;
}
/:
```

<div
  class="benchmark-card"
  data-title="Fibonacci benchmark"
  data-labels="gcc-14 -O3|cpl -O3|clang -O3|gcc-14 -O0|clang -O0|cpl -O0"
  data-values="0.412|0.412|0.417|0.421|0.430|0.434"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 32 lines (including comments such as a base block number). Were 48 lines.

## String iteration
```cpl
start() {
    str msg = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/";
    i64 outer = 0;
    u8 acc = 0;
    ptr i8 p = ref msg;

    while outer < 1000000; {
        p = ref msg;
        while dref p; {
            acc = (acc + dref p) & 0xFF;
            p += 1;
        }
        outer += 1;
    }

    exit acc;
}

:/ C version:
int main() {
    const char *msg = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/";
    long outer = 0;
    unsigned char acc = 0;
    const char *p = msg;

    while (outer < 1000000) {
        p = msg;
        while (*p) {
            acc = (unsigned char)((acc + (unsigned char)*p) & 0xFF);
            p += 1;
        }
        outer += 1;
    }

    return acc;
}
/:
```

<div
  class="benchmark-card"
  data-title="Pointer and string traversal benchmark"
  data-labels="clang -O3|gcc-14 -O3|cpl -O3|gcc-14 -O0|cpl -O0|clang -O0"
  data-values="0.430|0.470|0.513|0.566|0.603|1.002"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 87 lines (including comments such as a base block number). Were 103 lines.

## Brainfuck

```cpl
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }

    return l;
}

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

glob arr tape[30000, i8];
glob arr bracketmap[10000, i32];
glob arr stack[10000, i32];

start(i32 argc, ptr ptr i8 argv) {
    i32 pos = 0;
    i32 stackptr = 0;
    i32 codelength = strlen(argv[1]);
    while pos < codelength; {
        @[no_fall]
        @[straight]
        switch argv[1][pos]; {
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
        switch argv[1][pc]; {
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
            case '['; {
                if not tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            case ']'; {
                if tape[pointer]; pc = bracketmap[pc];
                else pc += 1;
            }
            default {
                pc += 1;
            }
        }
    }

    exit 0;
}

:/ C version:
#include <unistd.h>

char tape[30000];
int bracketmap[10000];
int stack[10000];

int my_strlen(char *s) {
    int l= 0;

    while (s[l]) {
        l+= 1;
    }

    return l;
}

void my_putc(char c) {
    write(1,&c,1);
}

int main(int argc,char **argv) {
    int pos= 0;
    int stackptr= 0;
    int codelength= my_strlen(argv[1]);

    while (pos < codelength) {
        switch (argv[1][pos]) {
            case '[': {
                stack[stackptr]= pos;
                stackptr+= 1;
                break;
            }

            case ']': {
                if (stackptr > 0) {
                    int matchpos;

                    stackptr-= 1;
                    matchpos= stack[stackptr];

                    bracketmap[pos]= matchpos;
                    bracketmap[matchpos]= pos;
                }
                break;
            }
        }

        pos+= 1;
    }

    int pc= 0;
    int pointer= 0;

    while (pc < codelength) {
        switch (argv[1][pc]) {
            case '>': {
                pointer+= 1;
                pc+= 1;
                break;
            }

            case '<': {
                pointer-= 1;
                pc+= 1;
                break;
            }

            case '+': {
                tape[pointer]+= 1;
                pc+= 1;
                break;
            }

            case '-': {
                tape[pointer]-= 1;
                pc+= 1;
                break;
            }

            case '.': {
                my_putc(tape[pointer]);
                pc+= 1;
                break;
            }

            case '[': {
                if (!tape[pointer]) {
                    pc= bracketmap[pc];
                } else {
                    pc+= 1;
                }
                break;
            }

            case ']': {
                if (tape[pointer]) {
                    pc= bracketmap[pc];
                } else {
                    pc+= 1;
                }
                break;
            }

            default: {
                pc+= 1;
                break;
            }
        }
    }

    return 0;
}
/:
```

<div
  class="benchmark-card"
  data-title="Brainfuck 'Oregon, CoosBay, I'm coming for you!' benchmark"
  data-labels="clang -O3|cpl -O3|cpl -O0|gcc-14 -O3|clang -O0|gcc-14 -O0"
  data-values="0.403|0.403|0.411|0.487|0.778|0.830"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 615 lines (including comments such as a base block number).  Were 769 lines.
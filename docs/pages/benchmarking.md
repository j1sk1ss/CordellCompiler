# CPL code performance evaluation
**Important note:** some microbenchmarks use `asm volatile` in the C version to prevent dead-code elimination of otherwise empty or non-observable loops. CPL currently preserves such loops by default, while optimizing C compilers may remove them. These tests should therefore be interpreted as loop-overhead measurements rather than general-purpose performance benchmarks. </br>

Versions:
- `gcc-14`: GCC 14.2.0 (Homebrew GCC 14.2.0)
    - Optimizations: `-O0`, `-O3`
- `clang`: Apple clang 12.0.0 (clang-1200.0.32.29)
    - Optimizations: `-O0`, `-O3`
- `cpl`: CPL v3.4 (MACHO64)
    - Optimizations: `-O0`, `-O3` (LICM, Peephole, CSE, DCE, Constant prop / fold) 
- `rustc`: rustc 1.87.0
    - Optimizations: `opt-level=0`, `opt-level=2`

Specs:
- OS: MacOS Catalina 10.15.7
- CPU: i7 3650QM, Quad-core, 2.4 GHz
- RAM: 16 GB, DDR3, 1600 MHz

Result gathering:
  - CPL - 5 times, every time after compilation, `py-time` total program execution time
  - Clang / GCC - 5 times, every time after compilation, `gnu-time` total execution time

*P.S.:* The results below can be considered as a valid value, but I'd suggest to add about `+10%` of consumed execution time for every CPL's result given possible issues in my measurement methods, hardware and software. Furthermore, such a suggestion is based on a `+%10` size difference between GCC's and Clang's `.asm` files with CPL's `.asm` files. </br>
*P.P.S.:* This section doesn't address how optimizations are affect on the final assembly which is produced by the compiler. If you're interest how it affects, please consider the related section in the documentation (which is `WIP`).

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

:/ Rust version
use std::arch::asm;
fn main() {
    let mut a: u32 = 0;
    while a < 1_000_000_000u32 {
        unsafe {
            asm!("/* {0:e} */", inout(reg) a, options(nomem, nostack));
        }
        a = a.wrapping_add(1);
    }
}
/:
```

The results below shows that the optimized CPL code has the same execution time as it have both the GCC's code and CLang's code.

<div
  class="benchmark-card"
  data-title="Empty loop benchmark"
  data-labels="rustc opt-level=2|cpl -O3|clang -O3|gcc-14 -O3|cpl -O0|rustc opt-level=0|gcc-14 -O0|clang -O0"
  data-values="0.333|0.702|0.748|0.758|1.39|3.442|4.290|4.641"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 11 lines. Were 31 lines.

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

:/ Rust version
use std::hint::black_box;
fn main() {
    let mut a: i32 = 1;
    let mut b: i32 = 0;
    for _ in 0..1_000_000u32 {
        let tmp = a;
        a = a.wrapping_add(b);
        b = tmp;
    }

    black_box(b);
}
/:
```

<div
  class="benchmark-card"
  data-title="Fibonacci benchmark"
  data-labels="rustc opt-level=2|rustc opt-level=0|gcc-14 -O3|cpl -O3|clang -O3|gcc-14 -O0|cpl -O0|clang -O0"
  data-values="0.335|0.337|0.412|0.412|0.417|0.421|0.424|0.430"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 17 lines. Were 40 lines.

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

:/ Rust version
use std::hint::black_box;
fn main() {
    let msg = black_box(b"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/\0");
    let limit = black_box(1_000_000i64);
    let mut outer: i64 = 0;
    let mut acc: u8 = 0;
    while outer < limit {
        let mut p = msg.as_ptr();

        unsafe {
            while *p != 0 {
                acc = acc.wrapping_add(*p);
                p = p.add(1);
            }
        }

        outer += 1;
    }

    black_box(acc);
}
/:
```

<div
  class="benchmark-card"
  data-title="Pointer and string traversal benchmark"
  data-labels="rustc opt-level=2|rustc opt-level=0|clang -O3|gcc-14 -O3|cpl -O3|gcc-14 -O0|cpl -O0|clang -O0"
  data-values="0.332|0.348|0.430|0.470|0.513|0.566|0.583|1.002"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 84 lines (including comments such as a base block number). Were 103 lines.

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

int _strlen(char *s) {
    int l= 0;
    while (s[l]) {
        l+= 1;
    }

    return l;
}

void _putc(char c) {
    write(1, &c, 1);
}

int main(int argc, char* argv[]) {
    int pos = 0;
    int stackptr = 0;
    int codelength = _strlen(argv[1]);

    while (pos < codelength) {
        switch (argv[1][pos]) {
            case '[': {
                stack[stackptr] = pos;
                stackptr += 1;
                break;
            }
            case ']': {
                if (stackptr > 0) {
                    int matchpos;
                    stackptr -= 1;
                    matchpos = stack[stackptr];
                    bracketmap[pos] = matchpos;
                    bracketmap[matchpos] = pos;
                }
                break;
            }
        }

        pos += 1;
    }

    int pc = 0;
    int pointer = 0;

    while (pc < codelength) {
        switch (argv[1][pc]) {
            case '>': {
                pointer += 1;
                pc += 1;
                break;
            }
            case '<': {
                pointer-= 1;
                pc += 1;
                break;
            }
            case '+': {
                tape[pointer]+= 1;
                pc += 1;
                break;
            }
            case '-': {
                tape[pointer]-= 1;
                pc += 1;
                break;
            }
            case '.': {
                _putc(tape[pointer]);
                pc += 1;
                break;
            }
            case '[': {
                if (!tape[pointer]) {
                    pc = bracketmap[pc];
                } else {
                    pc += 1;
                }
                break;
            }
            case ']': {
                if (tape[pointer]) {
                    pc = bracketmap[pc];
                } else {
                    pc += 1;
                }
                break;
            }
            default: {
                pc += 1;
                break;
            }
        }
    }

    return 0;
}
/:

:/ Rust version
use std::env;
use std::io::{self, Write};

static mut TAPE: [u8; 30000] = [0; 30000];
static mut BRACKETMAP: [i32; 10000] = [0; 10000];
static mut STACK: [i32; 10000] = [0; 10000];

fn putc(c: u8) {
    let mut out = io::stdout();
    out.write_all(&[c]).unwrap();
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        return;
    }

    let code = args[1].as_bytes();
    let codelength = code.len() as i32;
    let mut pos: i32 = 0;
    let mut stackptr: i32 = 0;

    unsafe {
        while pos < codelength {
            match code[pos as usize] {
                b'[' => {
                    STACK[stackptr as usize] = pos;
                    stackptr += 1;
                }
                b']' => {
                    if stackptr > 0 {
                        stackptr -= 1;
                        let matchpos = STACK[stackptr as usize];
                        BRACKETMAP[pos as usize] = matchpos;
                        BRACKETMAP[matchpos as usize] = pos;
                    }
                }
                _ => {}
            }

            pos += 1;
        }

        let mut pc: i32 = 0;
        let mut pointer: i32 = 0;
        while pc < codelength {
            match code[pc as usize] {
                b'>' => {
                    pointer += 1;
                    pc += 1;
                }
                b'<' => {
                    pointer -= 1;
                    pc += 1;
                }
                b'+' => {
                    TAPE[pointer as usize] = TAPE[pointer as usize].wrapping_add(1);
                    pc += 1;
                }
                b'-' => {
                    TAPE[pointer as usize] = TAPE[pointer as usize].wrapping_sub(1);
                    pc += 1;
                }
                b'.' => {
                    putc(TAPE[pointer as usize]);
                    pc += 1;
                }
                b'[' => {
                    if TAPE[pointer as usize] == 0 {
                        pc = BRACKETMAP[pc as usize];
                    } else {
                        pc += 1;
                    }
                }
                b']' => {
                    if TAPE[pointer as usize] != 0 {
                        pc = BRACKETMAP[pc as usize];
                    } else {
                        pc += 1;
                    }
                }
                _ => {
                    pc += 1;
                }
            }
        }
    }
}
/:
```

<div
  class="benchmark-card"
  data-title="Brainfuck 'Oregon, CoosBay, I'm coming for you!' benchmark"
  data-labels="rustc opt-level=2|rustc opt-level=0|clang -O3|cpl -O3|cpl -O0|gcc-14 -O3|clang -O0|gcc-14 -O0"
  data-values="0.328|0.340|0.403|0.403|0.45|0.487|0.778|0.830"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

**Note:** The execution code size (asm) of the CPL optimized file is 500 lines. Before the `-O3` were 699 lines.
# CPL Benchmarks
**Important note:** I will use `GCC` as the best case scenario. Also, I will add the `asm valotile` sections to force `GCC` save the code such as an empty loop (CPL doesn't delete such code, but GCC does). 

## Loops
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

<div
  class="benchmark-card"
  data-title="Empty loop benchmark"
  data-labels="gcc -O3|cpl --opt|cpl|gcc -O0"
  data-values="0.32|0.72|1.52|3.52"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

## Fibonacci
```cpl
@[naked] start() {
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
        asm volatile("" : "+r"(a), "+r"(b) : : "memory");
    }
    return b;
}
/:
```

<div
  class="benchmark-card"
  data-title="Fibonacci benchmark"
  data-labels="gcc -O3|gcc -O0|cpl --opt|cpl"
  data-values="0.008467|0.1|0.41|0.52"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

## Pointer and strings
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
```

<div
  class="benchmark-card"
  data-title="Fibonacci benchmark"
  data-labels="gcc -O3|gcc -O0|cpl --opt|cpl"
  data-values="0.03446|0.18|0.52|0.63"
  data-dataset-label="Runtime"
  data-y-label="Seconds"
  data-tooltip-suffix=" s"
>
  <div class="benchmark-chart-wrap">
    <canvas class="benchmark-chart"></canvas>
  </div>
</div>

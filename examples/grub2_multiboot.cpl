container framebuffer {
    u32 mode; 
    u32 x;
    u32 y;
    u32 bitness;

    @[self] 
    @[inline(model)]
    function init(ptr framebuffer self) -> i0 {
        self.mode    = 0;
        self.x       = 640;
        self.y       = 480;
        self.bitness = 32;
    }
}

container mb_header {
    u32         magic;
    u32         flags;
    u32         check;
    arr         padding[5, u32];
    framebuffer fb;

    @[self] 
    @[inline(model)]
    function init(ptr mb_header self) -> i0 {
        self.fb.init();
        self.magic = 0x1BADB002;
        self.flags = (1 << 0) | (1 << 1) | (1 << 2);
    }
}

@[section(".multiboot", 4)]
glob mb_header _header;

container stack {
    arr padding[16384, u8];
}

@[section(".bss", 16)] 
glob stack _stack;

function kernel_main(i32 argc, ptr ptr i8 argv) -> i0;

@[section(".text")]
@[entry("_start")]
@[naked]
function main() -> i0 {
    _header.init();

    asm(ref _stack) {
        "mov esp, %0",
        "push esp",
        "push eax",
        "push ebx",
        "cli",
        "xor ebp, ebp"
    }

    kernel_main(0, 0);

    asm() {
        ".hang: hlt",
        "jmp .hang"
    }
}
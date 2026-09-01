@[align(1)]
container framebuffer {
    usize mode; 
    usize x;
    usize y;
    usize bitness;

}

@[align(1)]
container mb_header {
    usize       magic;
    usize       flags;
    usize       check;
    arr         padding[5, usize];
    framebuffer fb;
}

@[section(".multiboot", 4)]
glob mb_header _header = { 
    :/ magic   /: 0x1BADB002, 
    :/ flags   /: 7, 
    :/ check   /: 3830599671, 
    :/ padding /: 0, 0, 0, 0, 0, 
    :/ mode    /: 0, 
    :/ x       /: 640, 
    :/ y       /: 480, 
    :/ bitness /: 32 
};

container stack {
    arr padding[16384, u8];
}

@[section(".bss", 16)] 
glob stack _stack;

@[section(".text")] function kmain(usize mb_info, usize magic, usize esp) -> i0;

@[section(".text")]
@[entry("_start")]
@[naked]
function main() -> i0 {
    @[register(0)] usize magic_src;
    @[register(1)] usize mb_info_src;
    usize magic   = magic_src;
    usize mb_info = mb_info_src;

    asm(ref _stack + sizeof(stack)) {
        "mov esp, %0",
        "cli",
        "xor ebp, ebp"
    }

    usize stack_top = 0;
    asm(stack_top) {
        "mov %0, esp"
    }

    kmain(mb_info, magic, stack_top);

    asm() {
        ".hang: hlt",
        "jmp .hang"
    }
}
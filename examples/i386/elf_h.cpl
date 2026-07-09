#ifndef ELF32_H_
#define ELF32_H_ 0

container elf32_symbol {
    u32 name_offset_in_strtab;
    u32 value;
    u32 size;
    u8  info;
    u8  other;
    u16 shndx;
}

container elf32_symbols {
    ptr elf32_symbol symtab;
    u32              symtab_size;
    ptr i8           strtab;
    u32              strtab_size;
}

container elf32_symbols_desctiptor {
    bool              present;
    uint32_t          num_symbols;
    ptr elf32_symbols symbols;
    ptr i8            string_table_addr;
}

#endif
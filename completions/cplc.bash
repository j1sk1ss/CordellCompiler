# bash completion for cplc.

_cplc_complete_files()
{
    if declare -F _filedir >/dev/null; then
        _filedir
    else
        COMPREPLY=( $(compgen -f -- "$cur") )
        compopt -o filenames 2>/dev/null || true
    fi
}

_cplc_complete_dirs()
{
    if declare -F _filedir >/dev/null; then
        _filedir -d
    else
        COMPREPLY=( $(compgen -d -- "$cur") )
        compopt -o filenames 2>/dev/null || true
    fi
}

_cplc()
{
    local cur prev
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD - 1]}"

    local options="
        -h --help
        -v --version
        -s --smth
        -E
        -I
        -D
        --print-stdlib-path
        --output
        --ast-analysis
        --ir-analysis
        --analysis-only
        --debug
        --no-debug
        --i-know-what-i-am-doing
        -O0 -O1 -O2 -O3
        --arch
        --asm-compiler
        --asm-format
        --linker
        --linker-mode
        -Xlinker --linker-arg
        -c --compile-only
        --linker-no-pie
        --linker-pie
        --linker-m32
        --linker-no-m32
        --entry-name
        --ro-section
        --glob-section
        --code-section
        --full-bytness
        --half-bytness
        --quart-bytness
        --eight-bytness
        --sys-type
        --tre
        --no-tre
        --finline
        --no-finline
        --licm
        --no-licm
        --constant
        --no-constant
        --copyprop
        --no-copyprop
        --peephole
        --no-peephole
        --emit-ast
        --emit-ir
        --emit-hir-cfg
        --emit-lir
        --emit-lir-cfg
        --emit-asm
        --emit-symtab
        --ast-output
        --ir-output
        --lir-output
        --asm-output
        -L
        -l
        -Wl,
    "

    case "$prev" in
        --arch)
            COMPREPLY=( $(compgen -W "x86_64 amd64 x86 i386 ia32" -- "$cur") )
            return
            ;;
        --sys-type)
            COMPREPLY=( $(compgen -W "unknown macho64 linux64 i386 windows64" -- "$cur") )
            return
            ;;
        --asm-format)
            COMPREPLY=( $(compgen -W "macho64 elf64 elf32 win64" -- "$cur") )
            return
            ;;
        --linker-mode)
            COMPREPLY=( $(compgen -W "c driver raw ld" -- "$cur") )
            return
            ;;
        --full-bytness|--half-bytness|--quart-bytness|--eight-bytness)
            COMPREPLY=( $(compgen -W "1 2 4 8" -- "$cur") )
            return
            ;;
        --emit-symtab)
            COMPREPLY=( $(compgen -W "var vars v fn func funcs fntb function functions sec sect section sections" -- "$cur") )
            return
            ;;
        -I|-L)
            _cplc_complete_dirs
            return
            ;;
        --output|--ast-output|--ir-output|--lir-output|--asm-output|--asm-compiler|--linker)
            _cplc_complete_files
            return
            ;;
        --emit-hir-cfg|--emit-lir-cfg|--entry-name|--ro-section|--glob-section|--code-section|-D|-Xlinker|--linker-arg|-l)
            return
            ;;
    esac

    case "$cur" in
        -*)
            COMPREPLY=( $(compgen -W "$options" -- "$cur") )
            return
            ;;
    esac

    _cplc_complete_files
}

complete -F _cplc cplc

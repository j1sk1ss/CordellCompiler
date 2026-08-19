#ifndef CONFIG_H_
#define CONFIG_H_

#include <std/mem.h>

typedef enum {
    UNKNOWN,
    MACHO64,
    LINUX64,
    I386,
    WINDOWS64
} arch_type_t;

typedef char* config_string_field_t;
typedef long  config_int_field_t;
typedef char  config_flag_field_t;

#define FLAG(name) config_flag_field_t name : 1
typedef struct {
    struct {
        /* How many errors we're planning to find. The lower value - than more sensitive 
           static analyser will be */
        config_int_field_t     attention;    /* 1, 2, 3 ...                           */
        /* How we tolerate dangerous syscalls and other things. The lower value - we less
           tolerant and block more compilations */
        config_int_field_t     acceptance;   /* 1, 2, 3 ...                           */
    } csa; /* CordellStaticAnalyzer */

    struct {
        config_string_field_t  entry_name;   /* main() name in the output ASM         */
        config_string_field_t  ro_section;   /* RO data section name, e.g. ".rodata"  */
        config_string_field_t  glob_section; /* Global data section, e.g. ".data"     */
        config_string_field_t  code_section; /* Code data section, e.g. ".text"       */
        struct {
            config_int_field_t bytness;      /* Max system's byte size, e.g. 8 - x64  */
            config_int_field_t h_bytness;    /* Half system byte size, e.g. 4 - x64   */
            config_int_field_t q_bytness;    /* Quart system byte size, e.g. 2 - x64  */
            config_int_field_t e_bytness;    /* Eight system byte size, e.g. 1 - x64  */
        } bytness;
        arch_type_t            sys_type;
        config_string_field_t  arch_timings; /* *.trg file with generated timings     */
    } system;

    struct {
        FLAG(tre);                           /* Enable or disable TRE optimization    */
        FLAG(finline);                       /* Enable or disable function inline     */
        FLAG(licm);                          /* Enable or disable LICM optimization   */
        FLAG(constant);                      /* Enable or disable constant fold/prop  */
        FLAG(peephole);                      /* Enable or disable peephole            */
    } optimization_flags;

    struct {
        FLAG(debug);                         /* Debug flag                            */
        FLAG(strict);                        /* The compiler stops on casts or not    */
        FLAG(parser_error);                  /* If there is a parser error            */
        FLAG(symtab_error);                  /* If there is a symtab error            */ 
    } compilation_flags;
} config_t;
#undef FLAG

void                  CONF_set_config(config_t conf);
config_string_field_t CONF_get_entry_name();
config_string_field_t CONF_get_ro_section();
config_string_field_t CONF_get_glob_section();
config_string_field_t CONF_get_code_section();
config_string_field_t CONF_get_no_section();
config_string_field_t CONF_get_timings();
config_int_field_t    CONF_get_full_bytness();
config_int_field_t    CONF_get_half_bytness();
config_int_field_t    CONF_get_quart_bytness();
config_int_field_t    CONF_get_eight_bytness();
config_int_field_t    CONF_get_attention_level();
config_int_field_t    CONF_get_acceptance_level();
config_flag_field_t   CONF_is_debug_compilation();
arch_type_t           CONF_get_system_type();
config_flag_field_t   CONF_is_strict_compilation();

/* Perform an action with a logic output with respect of the current
strict status in the compiler.
Provide it with an action which produces fail logic output as 'true'. 
Note: Must be as a source of a logic output. Otherwise it is redundant - It
      won't stop compilation by its own. */
#define STRICT_ACTION(fail_action) fail_action && CONF_is_strict_compilation()

config_flag_field_t CONF_is_parser_error();
void                CONF_set_parser_error();
config_flag_field_t CONF_is_symtab_error();
void                CONF_set_symtab_error();

#endif

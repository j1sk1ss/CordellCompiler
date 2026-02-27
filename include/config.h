#ifndef CONFIG_H_
#define CONFIG_H_

#include <std/mem.h>

typedef const char* config_string_field_t;
typedef const long  config_int_field_t;

typedef struct {
    struct {
        config_string_field_t  entry_name;   /* main() name in the output ASM        */
        config_string_field_t  ro_section;   /* RO data section name, e.g. ".rodata" */
        config_string_field_t  glob_section; /* Global data section, e.g. ".data"    */
        config_string_field_t  code_section; /* Code data section, e.g. ".text"      */
        struct {
            config_int_field_t bytness;      /* Max system's byte size, e.g. 8 - x64 */
            config_int_field_t h_bytness;    /* Half system byte size, e.g. 4 - x64  */
            config_int_field_t q_bytness;    /* Quart system byte size, e.g. 2 - x64 */
            config_int_field_t e_bytness;    /* Eight system byte size, e.g. 1 - x64 */
        } bytness;
    } system;

    struct {
        char                   tre      : 1; /* Enable or disable TRE optimization    */
        char                   finline  : 1; /* Enable or disable function inline     */
        char                   licm     : 1; /* Enable or disable LICM optimization   */
        char                   constant : 1; /* Enable or disable constant fold/prop  */
        char                   peephole : 1; /* Enable or disable peephole            */
    } optimization_flags;
} config_t;

int CONF_set_config(config_t* conf);

config_string_field_t CONF_get_entry_name();
config_string_field_t CONF_get_ro_section();
config_string_field_t CONF_get_glob_section();
config_string_field_t CONF_get_code_section();

config_int_field_t CONF_get_full_bytness();
config_int_field_t CONF_get_half_bytness();
config_int_field_t CONF_get_quart_bytness();
config_int_field_t CONF_get_eight_bytness();

#endif

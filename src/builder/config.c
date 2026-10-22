#include <builder.h>

config_t BUILD_make_config(const options_t* options) {
    config_t conf = {
        .system = {
            .entry_name     = options->config.entry_name,
            .ro_section     = options->config.ro_section,
            .glob_section   = options->config.glob_section,
            .code_section   = options->config.code_section,
            .bytness        = {
                .bytness    = options->config.full_bytness,
                .h_bytness  = options->config.half_bytness,
                .q_bytness  = options->config.quart_bytness,
                .e_bytness  = options->config.eight_bytness,
            },
            .sys_type       = options->config.sys_type,
        },
        .compilation_flags  = {
            .debug          = options->config.debug  ? 1 : 0,
            .strict         = options->config.strict ? 1 : 0,
            .parser_error   = 0,
            .symtab_error   = 0,
            .test           = options->flags.test_compilation ? 1 : 0
        },
    };

    return conf;
}

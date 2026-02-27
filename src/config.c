#include <config.h>

static config_t _config = { 0 };

int CONF_set_config(config_t* conf) {
    str_memcpy(&_config, conf, sizeof(config_t));
    return 1;
}

config_string_field_t CONF_get_entry_name() {
    if (!_config.system.entry_name) return "_main";
    return _config.system.entry_name;
}

config_string_field_t CONF_get_ro_section() {
    if (!_config.system.ro_section) return ".rodata";
    return _config.system.ro_section;
}

config_string_field_t CONF_get_glob_section() {
    if (!_config.system.glob_section) return ".data";
    return _config.system.glob_section;
}

config_string_field_t CONF_get_code_section() {
    if (!_config.system.code_section) return ".text";
    return _config.system.code_section;
}

config_int_field_t CONF_get_full_bytness() {
    if (!_config.system.bytness.bytness) return 8;
    return _config.system.bytness.bytness;
}

config_int_field_t CONF_get_half_bytness() {
    if (!_config.system.bytness.h_bytness) return 4;
    return _config.system.bytness.h_bytness;
}

config_int_field_t CONF_get_quart_bytness() {
    if (!_config.system.bytness.q_bytness) return 2;
    return _config.system.bytness.q_bytness;
}

config_int_field_t CONF_get_eight_bytness() {
    if (!_config.system.bytness.e_bytness) return 1;
    return _config.system.bytness.e_bytness;
}

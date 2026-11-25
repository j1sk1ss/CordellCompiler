#include <prep/token_types.h>

int main() {
    token_type_t types[] = {
        F32_TYPE_TOKEN, F32_VARIABLE_TOKEN, TMP_F32_TYPE_TOKEN,
        F64_TYPE_TOKEN, F64_VARIABLE_TOKEN, TMP_F64_TYPE_TOKEN,
        UNKNOWN_FLOAT_NUMERIC_TOKEN
    };
    for (int word = 4; word <= 8; word += 4) {
        config_t cfg = { .system.bytness = { .bytness = word } };
        CONF_set_config(cfg);
        for (int i = 0; i < (int)(sizeof(types) / sizeof(types[0])); i++) {
            token_t token = { .t_type = types[i] };
            type_size_t bitness = i < 3 ? TYPE_HALF_SIZE : TYPE_FULL_SIZE;
            long size = i < 3 ? 4 : word;
            if (TKN_variable_bitness(&token, 0) != bitness) return 1;
            if (TKN_convert_type_size(bitness) != size) return 2;
            token.flags.ptr = 2;
            if (TKN_variable_bitness(&token, 1) != TYPE_FULL_SIZE) return 3;
            if (TKN_convert_type_size(TKN_variable_bitness(&token, 1)) != word) return 4;
            if (TKN_variable_bitness(&token, 0) != bitness) return 5;
        }
    }
    return 0;
}

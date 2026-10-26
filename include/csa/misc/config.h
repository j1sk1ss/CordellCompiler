#ifndef CSA_CONFIG_H_
#define CSA_CONFIG_H_

typedef struct {
    struct {
        char provide_lines : 1;
        char provide_rows  : 1;
        char provide_files : 1;
    } l_conf;
} csa_conf_t;

#endif

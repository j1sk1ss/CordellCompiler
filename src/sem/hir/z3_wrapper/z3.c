#include <sem/hir/z3.h>

FILE* Z3_get_dump_file() {
    return NULL;
}

int Z3_delete_dump_file(FILE* f) {
    return 0;
}

const char* _get_dump_file_path(FILE* f) {
    return NULL;
}

static int _launch_z3(
    char* dump,
    char* function
) {

}

int Z3_can_vid_be_equal(symbol_id_t v_id, long long value) {
    return 0;
}

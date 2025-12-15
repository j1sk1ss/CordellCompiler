#include <lir/instplan/targinfo.h>

int TRGINF_load(char* path, target_info_t* s) {
    print_debug("TRGINF_load(path=%s)", path);
    
    map_init(&s->info, MAP_NO_CMP);
    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    fread(s, sizeof(s->name) + sizeof(int), 1, f);
    for (int i = 0; i < s->op_count; i++) {
        op_info_t* info = (op_info_t*)mm_malloc(sizeof(op_info_t));
        fread(info, sizeof(op_info_t), 1, f);
        map_put(&s->info, info->op, info);
        print_debug(
            "op=%i, comm=%i, issue=%i, latency=%i, rm=%i, sf=%i, th=%i, rf=%i, wm=%i", 
            info->op, info->commutative, info->issue_cost, 
            info->latency, info->reads_memory, info->sets_flags, 
            info->throughput, info->uses_flags, info->writes_memory
        );
    }

    fclose(f);
    return 1;
}

int TRGINF_unload(target_info_t* s) {
    return map_free_force(&s->info);
}

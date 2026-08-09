#include <std/logg.h>

int main() {
    log_message("TEST", "file.c", 12, "value=%d", 7);
    log_message(NULL, NULL, 0, NULL);
    return 0;
}

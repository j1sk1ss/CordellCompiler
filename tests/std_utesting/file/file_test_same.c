#include <std/file.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    string_t* this_file = create_string("std_utesting/file/file_test_same.c");
    string_t* same_file = create_string("std_utesting/file/file_test_same.c");
    string_t* other_file = create_string("std_testing.py");
    string_t* missing = create_string("std_utesting/file/no_such_file.c");
    string_t* no_slash = create_string("std_testing.py");
    string_t* no_slash_2 = create_string("module_testing.py");
    string_t* root_a = create_string("/tmp/a");
    string_t* root_b = create_string("/tmp/b");
    string_t manual_head = { 0 };
    manual_head.head = "std_testing.py";
    string_t* root_parent_a = create_string("/a");
    string_t* root_parent_b = create_string("/b");

    assert(is_same_file(this_file, same_file), "Same file wasn't recognized!");
    assert(!is_same_file(this_file, other_file), "Different files are same!");
    assert(!is_same_file(this_file, missing), "Missing file is same!");
    assert(!is_same_file(NULL, same_file), "NULL file path accepted!");
    assert(is_same_dir(this_file, same_file), "Same parent dir wasn't recognized!");
    assert(!is_same_dir(this_file, other_file), "Different parent dirs are same!");
    assert(is_same_dir(no_slash, no_slash_2), "Current dir parent wasn't recognized!");
    assert(is_same_dir(root_a, root_b), "Root parent dir wasn't recognized!");
    assert(is_same_file(&manual_head, no_slash), "Manual head path wasn't used!");
    assert(is_same_dir(root_parent_a, root_parent_b), "Slash root parent wasn't recognized!");
    assert(!is_same_dir(NULL, same_file), "NULL dir path accepted!");

    destroy_string(root_parent_b);
    destroy_string(root_parent_a);
    destroy_string(root_b);
    destroy_string(root_a);
    destroy_string(no_slash_2);
    destroy_string(no_slash);
    destroy_string(missing);
    destroy_string(other_file);
    destroy_string(same_file);
    destroy_string(this_file);
    return 0;
}

#ifndef CODE_EXT_05_LIST_INC_
#define CODE_EXT_05_LIST_INC_ 0

container node05 {
    ptr node05 next;
    i32 value;
}

container list05 {
    ptr node05 head;
    ptr node05 tail;

    @[self]
    function init(ptr list05 self) -> i0;

    @[self]
    function push_back(ptr list05 self, ptr node05 item) -> i0;

    @[self]
    function sum(ptr list05 self) -> i32;
}

#endif

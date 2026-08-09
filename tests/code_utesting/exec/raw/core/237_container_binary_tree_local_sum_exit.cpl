: BLOCK_TEST :
: RUN_ASM :

container tree_node {
    ptr tree_node left;
    ptr tree_node right;
    i32 value;

    @[self]
    function sum3(ptr tree_node self) -> i32 {
        return self.value + self.left.value + self.right.value;
    }
}

start() {
    tree_node root;
    tree_node left;
    tree_node right;

    root.value = 20;
    left.value = 9;
    right.value = 13;
    root.left = ref left;
    root.right = ref right;

    exit root.sum3() as u8;
}

:/ OUTPUT
@exit_code=42
/:

container node {
    ptr node next;
    i32      val;

    @[self] function init(ptr node self) -> i0 {
        self.next = 0 as ptr i0;
        self.val  = 0;
    }
}

start() {
    node nd;
    nd.init();
}

:/ OUTPUT
 2 | {
 5 |     function init(ptr node self) -> i0
 6 |     {
 6 |         self.next = 0 as ptr i0;
 7 |         self.val = 0;
 6 |     }
 1 |     container node
 2 |     {
 2 |         ptr node next;
 3 |         i32 val;
 2 |     }
11 |     start ()
12 |     {
12 |         node nd;
13 |         init(ref nd);
12 |     }
 2 | }
/:

container string {
    ptr i8 body;

    @[self]
    function new(ptr string self, ptr i8 data) -> i0 {
        self.body = data;
    }

    @[self]
    function strlen(ptr string self) -> i32 {
        i32 index = 0;
        ptr i8 s = self.body;
        while dref s; {
            index += 1;
            s += 1;
        }

        return index;
    }

    @[self]
    function equals(ptr string self, ptr string other) -> i8 {
        ptr i8 s1 = self.body;
        ptr i8 s2 = other.body;

        while dref s1 && dref s2; {
            if dref s1 != dref s2; return 0;
            s1 += 1;
            s2 += 1;
        }

        return (not dref s1 && not dref s2) as i8;
    }
}

@[entry("main")]
start() {
    string msg;
    msg.new(ref "Hello, World!");
    string other;
    other.new(ref "Hello, World!");
    if msg.equals(ref other); exit 1;
    exit 0;
}

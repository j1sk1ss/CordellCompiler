: RUN_ASM :
container string {
    ptr i8 body;

    @[self]
    function init(ptr string self, ptr i8 data) -> i0 {
        self.body = data;
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

start() {
    string msg;
    msg.init(ref "Hello, World!");
    string other;
    other.init(ref "Hello, World!");
    if msg.equals(ref other); exit 1;
    exit 0;
}

:/ OUTPUT
@exit_code=1
/:
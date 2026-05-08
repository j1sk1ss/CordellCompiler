: TEST_DEBUG :
function sum<O, R>(O a, R b) -> O {
    return a + b as O;
}

@[entry("_main")]
start() {
    exit sum<u8, u8>(10, 20);
}

:/ OUTPUT
/:
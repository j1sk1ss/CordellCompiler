@[entry]
function foo(i32 a) {
    if a > 10 && a <= 20; {
        return 1;
    }

    if a == 15; {
        return 10;
    }

    return 20;
}

:/ OUTPUT
[WARNING] [{X}:7:8] Can't reach the 'then' branch! Consider to refactor the code.
/:
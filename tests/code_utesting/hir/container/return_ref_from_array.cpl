: BUG :

container cell {
    i32 value;
}

container table {
    arr cells[2, cell];

    @[self]
    function get(ptr table self, i32 index) -> ptr cell;
}

function table::get(ptr table self, i32 index) -> ptr cell {
    return ref self.cells[index];
}

start() {
    table t;
    t.cells[0].value = 42 as i32;

    ptr cell c = t.get(0 as i32);
    exit c.value as u8;
}

:/ OUTPUT
{
{fn get0(u8* self, i32 index) -> u8*}
{i32t %11 = i32s %5 * csu8 8;}
{u8t* %14 = i8t* %12 as *ptr;}
{return u8t* %14;}
}
/:

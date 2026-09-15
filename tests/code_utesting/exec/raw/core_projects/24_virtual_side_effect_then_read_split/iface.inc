#ifndef CODE_EXT_24_IFACE_INC_
#define CODE_EXT_24_IFACE_INC_ 0
interface counter24 {
    @[self] @[abstract] function bump(ptr counter24 self, i32 delta) -> i0;
    @[self] @[abstract] function value(ptr counter24 self) -> i32;
}
#endif

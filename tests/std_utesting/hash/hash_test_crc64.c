#include <std/hash.h>
#include "../../misc/testing.h"

int main() {
    const unsigned char data[] = "123456789";
    unsigned long crc = crc64(data, 9, 0);
    assert(crc == 0x6C40DF5F0B497347UL, "CRC64 known vector failed!");
    assert(crc64(data, 0, 0x1234UL) == 0x1234UL, "CRC64 empty input failed!");
    return 0;
}

#include "reader.h"

Reader::Reader(std::string_view file) : std::ifstream(file.data(), std::ios::binary) {
    pos_ = 0;
}

bool Reader::GetBit() {
    if (pos_ == 0) {
        c_ = get();
    }
    bool ans;
    if ((c_ & (1 << (pos_))) == 0) {
        ans = false;
    } else {
        ans = true;
    }
    pos_++;
    if (pos_ == STANDARD_CHAR_SIZE) {
        pos_ = 0;
    }
    return ans;
}

uint16_t Reader::GetCntBits(int cnt) {
    uint16_t ans = 0;
    for (int i = 0; i < cnt; ++i) {
        uint16_t cur = 0;
        if (GetBit()) {
            cur = 1;
        }
        cur <<= (i);
        ans |= cur;
    }
    return ans;
}

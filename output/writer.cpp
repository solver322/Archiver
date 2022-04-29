#include "writer.h"

Writer::Writer(std::string_view file) : std::ofstream(file.data(), std::ios::binary) {
    pos_ = 0;
    c_ = 0;
}

void Writer::WriteBit(bool x) {
    if (x) {
        c_ |= (1 << (pos_));
    }
    pos_++;
    if (pos_ == STANDARD_CHAR_SIZE) {
        put(c_);
        pos_ = 0;
        c_ = 0;
    }
}

void Writer::WriteCntBits(uint16_t x, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        if (((x >> (i)) & 1) == 0) {
            WriteBit(false);
        } else {
            WriteBit(true);
        }
    }
}

void Writer::WriteVector(std::vector<bool>& v) {
    for (auto u : v) {
        WriteBit(u);
    }
}

void Writer::Close() {
    put(c_);
    pos_ = 0;
    basic_ofstream::close();
}

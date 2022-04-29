#pragma once
#include <fstream>

const int CNT_BITS = 9;
const int STANDARD_CHAR_SIZE = 8;

class Reader : public std::ifstream {
public:
    Reader(std::string_view file);

    bool GetBit();
    uint16_t GetCntBits(int cnt = CNT_BITS);

private:
    unsigned char c_;
    size_t pos_;
};

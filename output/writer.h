#pragma once
#include <fstream>
#include <vector>

const int CNT_BITS = 9;
const int STANDARD_CHAR_SIZE = 8;

class Writer : public std::ofstream {
public:
    Writer(std::string_view file);

    void Close();
    void WriteBit(bool x);
    void WriteCntBits(uint16_t, int cnt = CNT_BITS);
    void WriteVector(std::vector<bool> &v);

private:
    unsigned char c_;
    int pos_;
};

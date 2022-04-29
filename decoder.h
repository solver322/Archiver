#pragma once

#include <vector>
#include <cstdint>
#include "input/reader.h"

class Decoder {
public:
    Decoder(std::string &archive_name);

private:
    std::vector<std::pair<int, uint16_t>> GetCodes(Reader &in) const;
    std::vector<std::pair<size_t, int>> GetLenCnt(Reader &in) const;
    uint16_t GetSymbol(Reader &in, Node cur) const;
    void Write(Reader &in, Node &head);
    uint16_t cnt_symbols_;
    std::string archive_name_;
};

#pragma once

#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include "huffman/huffman.h"

class Encoder {
public:
    Encoder(std::string &archive_name, std::vector<std::string> &files);
    void Start();

private:
    std::map<uint16_t, int> GetSymbols() const;
    void Write(Huffman &h, size_t max_code_size, std::map<size_t, int> &len_cnt) const;
    std::vector<std::string> file_names_;
    std::string archive_name_;
};

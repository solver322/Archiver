#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "encoder.h"
#include "output/writer.h"
#include "const/io.h"

std::map<uint16_t, int> Encoder::GetSymbols() const {
    std::map<uint16_t, int> symbols;
    for (size_t i = 0; i < file_names_.size(); ++i) {
        for (size_t j = 0; j < file_names_[i].size(); ++j) {
            symbols[file_names_[i][j]]++;
        }
        symbols[FILENAME_END]++;
        if (i + 1 != file_names_.size()) {
            symbols[ONE_MORE_FILE]++;
        }

        std::ifstream in(file_names_[i].data(), std::ios::binary);
        unsigned char c = in.get();

        while (in.good()) {

            symbols[c]++;
            c = in.get();
        }
    }
    symbols[ARCHIVE_END]++;
    return symbols;
}

Encoder::Encoder(std::string &archive_name, std::vector<std::string> &file_names) {
    file_names_ = file_names;
    archive_name_ = archive_name;
}

void Encoder::Start(){
    std::map<uint16_t, int> symbols = GetSymbols();

    Huffman h;
    h.BuildTreeEncode(symbols);

    size_t max_code_size = 0;
    std::map<size_t, int> len_cnt;

    for (auto u : h.codes_) {
        len_cnt[h.canonic_codes_[u.second].size()]++;
        max_code_size = std::max(max_code_size, h.canonic_codes_[u.second].size());
    }

    Write(h, max_code_size, len_cnt);
}

void Encoder::Write(Huffman &h, size_t max_code_size, std::map<size_t, int> &len_cnt) const {
    Writer out(archive_name_);

    out.WriteCntBits(static_cast<uint16_t>(h.codes_.size()));

    for (auto u : h.codes_) {
        out.WriteCntBits(u.second);
    }

    for (size_t i = 1; i <= max_code_size; ++i) {
        out.WriteCntBits(len_cnt[i], 8);
    }

    for (size_t i = 0; i < file_names_.size(); ++i) {
        for (size_t j = 0; j < file_names_[i].size(); ++j) {
            out.WriteVector(h.canonic_codes_[file_names_[i][j]]);
        }
        out.WriteVector(h.canonic_codes_[FILENAME_END]);

        std::ifstream in(file_names_[i].data(), std::ios::binary);
        unsigned char c = in.get();
        while (in.good()) {
            out.WriteVector(h.canonic_codes_[c]);
            c = in.get();
        }

        if (i + 1 != file_names_.size()) {
            out.WriteVector(h.canonic_codes_[ONE_MORE_FILE]);
        }
    }

    out.WriteVector(h.canonic_codes_[ARCHIVE_END]);
    out.close();
}

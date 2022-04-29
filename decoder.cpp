#include <string>
#include <vector>
#include "encoder.h"
#include "input/reader.h"
#include "decoder.h"
#include "const/io.h"

std::vector<std::pair<int, uint16_t>> Decoder::GetCodes(Reader &in) const {
    std::vector<std::pair<int, uint16_t>> codes(cnt_symbols_);

    for (size_t i = 0; i < codes.size(); ++i) {
        codes[i].second = in.GetCntBits();
    }
    return codes;
}

std::vector<std::pair<size_t, int>> Decoder::GetLenCnt(Reader &in) const {
    uint16_t sum = 0;
    std::vector<std::pair<size_t, int>> len_cnt;
    for (size_t i = 1;; ++i) {
        int cur = in.GetCntBits(STANDARD_CHAR_SIZE);
        if (cur != 0) {
            len_cnt.push_back({i, cur});
            sum += cur;
        }

        if (sum == cnt_symbols_) {
            break;
        }
    }

    return len_cnt;
}

uint16_t Decoder::GetSymbol(Reader &in, Node cur) const {
    uint16_t symbol;
    while (true) {
        if (cur.is_term) {
            symbol = cur.symbol;
            break;
        }
        if (in.GetBit()) {
            cur = *cur.right_son;
        } else {
            cur = *cur.left_son;
        }
    }
    return symbol;
}

void Decoder::Write(Reader &in, Node &head) {
    std::string file_name;
    bool file_open = false;
    std::ofstream out;

    while (true) {
        uint16_t symbol = GetSymbol(in, head);

        if (symbol == ARCHIVE_END) {
            break;
        } else if (symbol == FILENAME_END) {
            file_open = true;
            out = std::ofstream(file_name.data(), std::ios::binary);
        } else if (symbol == ONE_MORE_FILE) {
            file_name = "";
            file_open = false;
        } else if (!file_open) {
            file_name += static_cast<char>(symbol);
        } else {
            out.put(static_cast<char>(symbol));
        }
    }
}

Decoder::Decoder(std::string &archive_name) {
    archive_name_ = archive_name;
    Reader in(archive_name_);
    cnt_symbols_ = in.GetCntBits();

    std::vector<std::pair<int, uint16_t>> codes = GetCodes(in);

    std::vector<std::pair<size_t, int>> len_cnt = GetLenCnt(in);

    Huffman h;
    Node head = h.BuildTreeDecode(codes, len_cnt);
    Write(in, head);
}

#pragma once
#include <vector>
#include <cstdint>
#include <map>
#include "node.h"

class Heap {
public:
    void Push(std::pair<int, Node*>);
    void Pop(size_t);
    std::pair<int, Node*> Top() const;
    bool Empty() const;

private:
    std::vector<std::pair<int, Node*>> h_;
    int sz_ = 0;
};

class Huffman {
    void DescentTrie(Node* p_node);
    void BuildCanonicCodes();

public:
    void BuildTreeEncode(const std::map<uint16_t, int> &symbols);
    Node BuildTreeDecode(const std::vector<std::pair<int, uint16_t>> &codes,
                         const std::vector<std::pair<size_t, int>> &len_cnt);
    int path_ = 0;
    std::map<uint16_t, std::vector<bool>> canonic_codes_;
    std::vector<std::pair<int, uint16_t>> codes_;
};

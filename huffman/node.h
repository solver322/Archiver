#pragma once
#include <cstdint>

class Node {
public:
    uint16_t symbol;
    bool is_term = false;
    Node* left_son = nullptr;
    Node* right_son = nullptr;

    Node(uint16_t sym);
    Node(Node* l, Node* r);
    ~Node();
};

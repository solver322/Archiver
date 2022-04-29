#include "node.h"

Node::Node(Node *l, Node *r) {
    left_son = l;
    right_son = r;
}
Node::Node(uint16_t sym) {
    symbol = sym;
    is_term = true;
}
Node::~Node() {
    if (left_son != nullptr) {
        delete left_son;
    }
    if (right_son != nullptr) {
        delete right_son;
    }
}

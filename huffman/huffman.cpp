#include <queue>
#include <cstdint>
#include <algorithm>
#include <map>
#include "huffman.h"

void Heap::Push(std::pair<int, Node*> x) {
    sz_++;
    h_.push_back(x);
    int cur = h_.size() - 1;
    int par = cur / 2;
    while (cur != 0) {
        if (h_[cur].first <= h_[par].first) {
            break;
        } else {
            std::swap(h_[cur], h_[par]);
            cur = par;
            par = cur / 2;
        }
    }
}

void Heap::Pop(size_t x) {
    if (x == 0) {
        sz_--;
        h_[x].first = -1000000000;
    }
    if (x * 2 >= h_.size()) {
        return;
    }
    if (x * 2 + 1 == h_.size()) {
        std::swap(h_[x], h_[x * 2]);
        h_.pop_back();
    } else {
        if (h_[x * 2].first > h_[x * 2 + 1].first) {
            std::swap(h_[x * 2], h_[x]);
            Pop(x * 2);
        } else {
            std::swap(h_[x * 2 + 1], h_[x]);
            Pop(x * 2 + 1);
        }
    }
}

std::pair<int, Node*> Heap::Top() const {
    if (Empty()) {
        return {0, nullptr};
    }
    return h_[0];
}

bool Heap::Empty() const {
    if (sz_ == 0) {
        return true;
    }
    return false;
}

void Add(std::vector<bool> &path) {
    for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
        if (!path[i]) {
            path[i] = true;
            return;
        }
        path[i] = false;
    }
}

Node *GetTermVertex(Node &cur, std::vector<bool> &path, size_t i) {
    if (i == path.size()) {
        return &cur;
    }
    if (path[i]) {
        if (cur.right_son == nullptr) {
            cur.right_son = new Node(nullptr, nullptr);
        }
        return GetTermVertex(*cur.right_son, path, i + 1);
    } else {
        if (cur.left_son == nullptr) {
            cur.left_son = new Node(nullptr, nullptr);
        }
        return GetTermVertex(*cur.left_son, path, i + 1);
    }
}

Node Huffman::BuildTreeDecode(const std::vector<std::pair<int, uint16_t>> &codes,
                              const std::vector<std::pair<size_t, int>> &len_cnt) {
    codes_ = codes;

    size_t sum = 0;
    int it = -1;
    std::vector<bool> path;

    Node head(nullptr, nullptr);
    for (size_t i = 0; i < codes_.size(); ++i) {
        if (i == sum) {
            it++;
            sum += len_cnt[it].second;
            while (path.size() != len_cnt[it].first) {
                path.push_back(false);
            }
        }
        canonic_codes_[codes_[i].second] = path;

        Node *term = GetTermVertex(head, path, 0);
        term->symbol = codes_[i].second;
        term->is_term = true;

        Add(path);
    }
    return head;
}

void Huffman::BuildTreeEncode(const std::map<uint16_t, int> &symbols) {
    Heap pq;

    for (auto it = --symbols.end();; it--) {
        pq.Push({-it->second, new Node(it->first)});
        if (it == symbols.begin()) {
            break;
        }
    }

    Node *head = nullptr;
    while (!pq.Empty()) {
        std::pair<int, Node*> lft = pq.Top();
        pq.Pop(0);
        if (pq.Empty()) {
            head = lft.second;
            break;
        }
        std::pair<int, Node*> rgh = pq.Top();
        pq.Pop(0);

        int sum = lft.first + rgh.first;
        pq.Push({sum, new Node(lft.second, rgh.second)});
    }


    DescentTrie(head);

    BuildCanonicCodes();
    delete head;
}

bool CompareCodes(std::pair<int, uint16_t> &a, std::pair<int, uint16_t> &b) {
    if (a.first != b.first) {
        return a.first < b.first;
    }
    return a.second < b.second;
}

void Huffman::BuildCanonicCodes() {
    std::sort(codes_.begin(), codes_.end(), CompareCodes);

    std::vector<bool> path;
    for (size_t i = 0; i < codes_.size(); ++i) {
        while (static_cast<int>(path.size()) != codes_[i].first) {
            path.push_back(false);
        }
        canonic_codes_[codes_[i].second] = path;
        Add(path);
    }
    /*
        for (size_t i = 0; i < codes_.size(); ++i) {
            std::cout << canonic_codes_[i].second << " ";
            for (size_t j = 0; j < canonic_codes_[i].first.size(); ++j) {
                if (canonic_codes_[i].first[j]) {
                    std::cout << "1";
                } else {
                    std::cout << "0";
                }
            }
            std::cout << "\n";
        }
        */
}

void Huffman::DescentTrie(Node *cur_vertex) {
    if (cur_vertex->left_son != nullptr) {
        path_++;
        DescentTrie(cur_vertex->left_son);
        DescentTrie(cur_vertex->right_son);
        path_--;
    } else {
        codes_.push_back({path_, cur_vertex->symbol});
    }
}

#include <catch.hpp>

#include "huffman/huffman.h"
#include "encoder.h"
#include "decoder.h"

TEST_CASE("FillHeap") {
    Heap h;

    const std::pair<int, Node*> push1 = {1, new Node(nullptr, nullptr)};
    const std::pair<int, Node*> push2 = {2, new Node(nullptr, nullptr)};
    const std::pair<int, Node*> push3 = {3, new Node(nullptr, nullptr)};
    const std::pair<int, Node*> push4 = {4, new Node(nullptr, nullptr)};
    h.Push(push1);
    h.Push(push2);
    h.Push(push3);

    REQUIRE(push3 == h.Top());

    h.Pop(0);
    REQUIRE(push2 == h.Top());

    h.Pop(0);
    h.Pop(0);
    REQUIRE(true == h.Empty());

    h.Push(push4);
    REQUIRE(push4 == h.Top());

    h.Pop(0);
    REQUIRE(true == h.Empty());
}

TEST_CASE("FillHuffmanEncodeEasy") {
    Huffman h;

    const std::map<uint16_t, int> symbols = {{'a', 3}, {'b', 4}, {'c', 10}};

    h.BuildTreeEncode(symbols);

    const std::map<uint16_t, std::vector<bool>> canonic_codes = {
        {'c', {false}}, {'b', {true, true}}, {'a', {true, false}}};
    REQUIRE(canonic_codes == h.canonic_codes_);

    const std::vector<std::pair<int, uint16_t>> codes = {{1, 'c'}, {2, 'a'}, {2, 'b'}};
    REQUIRE(codes == h.codes_);
}

TEST_CASE("FillHuffmanEncodeHard") {
    Huffman h;

    const std::map<uint16_t, int> symbols = {{'a', 3}, {'b', 4}, {'c', 10}, {'d', 100}, {'e', 15}, {'f', 6}, {'g', 33}};

    h.BuildTreeEncode(symbols);

    const std::map<uint16_t, std::vector<bool>> canonic_codes = {
        {'g', {1, 0}},       {'f', {1, 1, 1, 1, 0}},    {'e', {1, 1, 0}},         {'d', {0}},
        {'c', {1, 1, 1, 0}}, {'b', {1, 1, 1, 1, 1, 1}}, {'a', {1, 1, 1, 1, 1, 0}}};
    REQUIRE(canonic_codes == h.canonic_codes_);

    const std::vector<std::pair<int, uint16_t>> codes = {{1, 'd'}, {2, 'g'}, {3, 'e'}, {4, 'c'},
                                                         {5, 'f'}, {6, 'a'}, {6, 'b'}};
    REQUIRE(codes == h.codes_);
}

TEST_CASE("FillHuffmanDecodeEasy") {
    Huffman h;

    const std::vector<std::pair<int, uint16_t>> codes = {{1, 'c'}, {2, 'a'}, {2, 'b'}};

    const std::vector<std::pair<size_t, int>> len_cnt = {{1, 1}, {2, 2}};

    h.BuildTreeDecode(codes, len_cnt);

    const std::map<uint16_t, std::vector<bool>> canonic_codes = {
        {'c', {false}}, {'b', {true, true}}, {'a', {true, false}}};

    REQUIRE(canonic_codes == h.canonic_codes_);
}

TEST_CASE("FillHuffmanDecodeHard") {
    Huffman h;

    const std::vector<std::pair<int, uint16_t>> codes = {{1, 'd'}, {2, 'g'}, {3, 'e'}, {4, 'c'},
                                                         {5, 'f'}, {6, 'a'}, {6, 'b'}};

    const std::vector<std::pair<size_t, int>> len_cnt = {{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 2}};

    h.BuildTreeDecode(codes, len_cnt);

    const std::map<uint16_t, std::vector<bool>> canonic_codes = {
        {'g', {1, 0}},       {'f', {1, 1, 1, 1, 0}},    {'e', {1, 1, 0}},         {'d', {0}},
        {'c', {1, 1, 1, 0}}, {'b', {1, 1, 1, 1, 1, 1}}, {'a', {1, 1, 1, 1, 1, 0}}};

    REQUIRE(canonic_codes == h.canonic_codes_);
}

TEST_CASE("EncoderDecoderEasy") {

    std::vector<std::string> file_names = {"file1", "file2"};

    std::vector<std::string> file_texts = {"ab", "c"};

    for (size_t i = 0; i < file_names.size(); ++i) {
        std::ofstream file(file_names[i]);
        file << file_texts[i];
        file.close();
    }

    std::string archive = "archi";
    Encoder e(archive, file_names);
    e.Start();
    Decoder d(archive);

    for (size_t i = 0; i < file_names.size(); ++i) {
        std::ifstream in(file_names[i].data());

        std::string word;

        unsigned char c = in.get();

        while (in.good()) {
            word += c;
            c = in.get();
        }
        REQUIRE(word == file_texts[i]);
    }
}

TEST_CASE("EncoderDecoderHard") {

    std::vector<std::string> file_names = {"file1", "file2"};

    std::vector<std::string> file_texts = {
        "abc;jsdfvb;jbsovnwn;cksdaoivneoivb\n"
        "kjsdnfv;ldsnfv;sfd\n"
        "ofvndso;vnsdnveakln;v;er;invsdvdf\n"
        "s;lkvn;dfnv'sndfvndv\n"
        "2iwabfei03q4hfrionsb309whg3ibepos;nvf0[fs3inrvsdf\n",
        "xxxksdnfvo;sdboienvone;svnd\n"
        ";lsdkfnv;sdnvsdnfvv;sklndflkdsv\n"
        "f;lvnsdo;vndso;vnf\n"
        ";fdnv;sdfnvo;fsdivonraevsnd;fds\n"
        "sadvoindovbsdf;vbaenrioawne013r9032jrh835gbupsnfvoiensv\n"
        "oebg0q248fnarsnd[oiawnovnoi[va09f[w;ekadsvmaesd\n"};

    for (size_t i = 0; i < file_names.size(); ++i) {
        std::ofstream file(file_names[i]);
        file << file_texts[i];
        file.close();
    }

    std::string archive = "archi";
    Encoder e(archive, file_names);
    e.Start();
    Decoder d(archive);

    for (size_t i = 0; i < file_names.size(); ++i) {
        std::ifstream in(file_names[i].data());

        std::string word;

        unsigned char c = in.get();

        while (in.good()) {
            word += c;
            c = in.get();
        }
        REQUIRE(word == file_texts[i]);
    }
}

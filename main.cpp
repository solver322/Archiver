#include <iostream>
#include <cstring>
#include "huffman/huffman.h"
#include "encoder.h"
#include "decoder.h"

void PrintReference();
int main(int argc, char* argv[]) {
    if (argc <= 1) {
        PrintReference();
    } else if (std::strcmp(argv[1], "-c") == 0) {
        std::vector<std::string> file_names;
        for (int i = 3; i < argc; ++i) {
            file_names.push_back(argv[i]);
        }
        std::string archive_name = argv[2];
        Encoder e(archive_name, file_names);
        e.Start();
    } else if (std::strcmp(argv[1], "-d") == 0) {
        std::string archive_name = argv[2];
        Decoder d(archive_name);
    } else {
        PrintReference();
    }
}
void PrintReference() {
    std::cout << "-c archive_name file1 file2 ... - compress files file1, file2, ... into archive archive_name,"
                 "\n"
                 "-d archive_name - decompress files from archive_name and save them with initial names in the current "
                 "directory,"
                 "\n"
                 "-h - display help on using the program.\n"
                 "\n";
}

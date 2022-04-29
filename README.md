# archiver

Console application for file compression


## Description

The program implements the archiver using the [Huffman algorithm](https://en.wikipedia.org/wiki/Huffman_coding).


The archiver program has the following command line interface:
* `archiver -c archive_name file1 [file2 ...]` - archive files `fil1, file2, ...` and save result to file `archive_name`.
* `archiver -d archive_name` - unarchive files from the archive `archive_name` and put in the current directory. File names must be preserved when archiving and unarchiving.
* `archiver -h` - display help on using the program.

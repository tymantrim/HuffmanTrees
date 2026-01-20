This project is a program that uses a Huffman Tree to compress text. This process involves totaling the frequency of each character in the input file, 
and utilizing a priority queue to turn these characters into a binary tree based on these frequencies. Each character is then represented by the binary 
path (0 means taking the left child, 1 means taking the right child) from the root to each character. The input file is then compressed into 
<input file>_c.txt by writing the binary paths to the file, where each set of 8 bits becomes a character. From there, the tree is stored using 
binary serialization.

For decompression, the compressed file and the tree file are taken, the tree is restructured using binary deserialization, and each bit of the 
compressed file is used to step throught the tree. When a character node is reached on the tree, that character is added to the output file and 
the program returns to the root of the tree. This is repeated until the end of the compressed file.

Usage details:
Begin by compiling the program (HuffmanTree.cpp), an example compilation command is: g++ -std=c++17 -Wall -Wextra -O2 HuffmanTree.cpp

From there, the program can be ran in two ways: a compression mode and a decompression mode
For compression, it is to be ran as follows:
./<executable name> -i <input file> -c

For decompression, it is to be ran as follows:
./<executable name> -i <compressed file> -t <tree file> -d

Compression statistics:
Below will be some comparisons of file size before and after compression. Size of the tree file will be included in all "after" metrics

"Hello, World!"
Before: 13B
After: 63B (5B + 58B)
Compression Ratio: 485%

Lyrics to the song Marcel by Her's
Before: 379B
After: 468B (218B + 250B)
Compression Ratio: 123%

Wiki article on the Elden Ring Charater Mohg
Before: 20.7KB
After: 12.3KB (11.8KB + 472B)
Compression Ratio: 59.4%

Book of Genesis
Before: 194KB
After: 114KB (114KB + 448B)
Compression Ratio: 58.8%

Lyrics to the song Virgin Suicide by Kinoko Teikoku in original Japanese Kanji
Before: 665B
After: 797B (385B + 412B)
Compression Ratio: 120%

From these few tests, we can see that even in small text files, the contents of the compressed file are significantly smaller, usually in the ballpark of around
half the size, however the size of the tree file necessary for decompression holds back this compression size. As the file being compressed grows larger however,
we see that the size of the tree file grows much less, meaning it takes less of the total compression size and allows for a compression ratio of around 60%.
We can also see from these tests that the script or alphabet of the input text does not matter, as it was able to compress and decompress Japanese Kanji as well 
as the Latin alphabet.

Future developements:
If I were to return to this project in the future, I would focus on shrinking the size of tree files, in order to improve the compression ratio for smaller files.
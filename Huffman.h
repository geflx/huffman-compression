#ifndef HUFFMAN_H
#define HUFFMAN_H

// Data Structures.
#include <vector>
#include <queue>
#include <string>

/* 
    @author = Gabriel Felix
    @date = June/2019
    @site = github.com/geflx/huffman-compression
*/

class HuffNode {
public:

    // HuffNode: char frequency, char and bool indicator if node is a char end.
    HuffNode(int freqs, char c_ = '0', bool isChar_ = false)
        : isChar(isChar_)
        , freq(freqs)
        , c(c_)
        , right(NULL)
        , left(NULL){};

    int freq;

    char c;
    bool isChar;

    HuffNode* right;
    HuffNode* left;
};

class HuffManTree {

public:

    HuffManTree(int freqs[]);
    HuffManTree(const HuffManTree& obj);
    ~HuffManTree();

    HuffManTree& operator=(const HuffManTree& obj);

    void compress(std::vector<bool>& out, const std::vector<char>& in) const;
    void decompress(std::vector<char>& out, const std::vector<bool>& in) const;

private:

    void compressionPreProcessing();
    void preProcessingAuxiliar(HuffNode* ptr, const std::string& temp);

    // Char frequency: 256 chars + 1 position indicating total bits.
    std::vector<int> freq = std::vector<int>(257, 0);
    std::vector<std::vector<bool>> bits = std::vector<std::vector<bool>>(256);
    HuffNode* root;

    HuffNode* copyNodes(const HuffNode* obj);

    void destroy(HuffNode* ptr);
    void create();
};
#endif
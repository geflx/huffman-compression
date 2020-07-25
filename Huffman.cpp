#include "Huffman.h"
#include <iostream>

/* 
    @author = Gabriel Felix
    @date = June/2019
    @site = github.com/geflx/huffman-compression
*/

HuffManTree::HuffManTree(int freqs[])
{
    root = NULL;
    for (int i = 0; i < 257; i++) // Copying char frequencies.
        freq[i] = freqs[i];

    std::priority_queue<HuffNode*> pq;

    for (int j = 0; j < 256; j++) { // Create HuffNodes for each used char.

        if (freqs[j] > 0) {
            char temporary = j;
            HuffNode* auxiliar = new HuffNode(freqs[j], temporary, true);
            pq.push(auxiliar);
        }

        freq[j] = freqs[j];
    }
    while (pq.size() > 1) { // Joining nodes and create a HuffMan Tree.

        HuffNode* nodeA = pq.top();
        pq.pop();

        HuffNode* nodeB = pq.top();
        pq.pop();

        HuffNode* nodeC = new HuffNode(nodeA->freq + nodeB->freq, '0', false);
        nodeC->left = nodeA;
        nodeC->right = nodeB;
        pq.push(nodeC);

    }

    HuffNode* ptr = pq.top();
    root = pq.top(); // Refreshing root.
    
    // Preprocessing bits for each used char.
    compressionPreProcessing();
}

HuffManTree::HuffManTree(const HuffManTree& obj)
{
    create();
    *this = obj;
}

HuffManTree& HuffManTree::operator=(const HuffManTree& obj)
{
    if (this == &obj)
        return *this;

    destroy(root);
    root = copyNodes(obj.root);

    freq = obj.freq;
    bits = obj.bits;
    return *this;
}

HuffNode* HuffManTree::copyNodes(const HuffNode* ptr)
{
    if (ptr == NULL)
        return NULL;

    HuffNode* ans = new HuffNode(ptr->freq, ptr->c, ptr->isChar);
    ans->right = copyNodes(ptr->right);
    ans->left = copyNodes(ptr->left);

    return ans;
}

void HuffManTree::create()
{
    root = NULL;
}

void HuffManTree::destroy(HuffNode* ptr)
{
    if (!ptr)
        return;

    destroy(ptr->right);
    destroy(ptr->left);

    delete ptr;
}

HuffManTree::~HuffManTree()
{
    destroy(root);
}

void HuffManTree::compressionPreProcessing()
{
    // Storage boolean arrays in HuffNodes that ARE chars. It also uses a recursive auxiliar function.
    std::string temp = "";

    if (root->isChar)
        preProcessingAuxiliar(root, temp + "0");
    else 
        preProcessingAuxiliar(root, temp);
    
}

void HuffManTree::preProcessingAuxiliar(HuffNode* ptr, const std::string& temp)
{
    // Stores '1' or '0' in boolean arrays during recursive path.
    if (!ptr)
        return;

    // Saving progress if I am a HuffNode which represents a char.
    if (ptr->isChar)
        for (int i = 0; i < temp.size(); i++) 

            if (temp[i] == '1') 
                bits[(unsigned char)ptr->c].push_back(true);
            else 
                bits[(unsigned char)ptr->c].push_back(false);
            
        
    // Go left and adds '0'.
    if (ptr->left) 
        preProcessingAuxiliar(ptr->left, temp + "0");
    
    // Go right and adds '1'.
    if (ptr->right) 
        preProcessingAuxiliar(ptr->right, temp + "1");
    
}

void HuffManTree::compress(std::vector<bool>& out, const std::vector<char>& in) const
{
    // Convert input char array in boolean output array.
    for (int i = 0; i < in.size(); i++)
        for (int j = 0; j < bits[(unsigned char)in[i]].size(); j++) 

            if (bits[(unsigned char)in[i]][j]) 
                out.push_back(true);
            else 
                out.push_back(false);

    return;
}

void HuffManTree::decompress(std::vector<char>& out, const std::vector<bool>& in) const
{

    /*  1 - Start from root.
        2 - If boolean input is '1' go left, if '0' go right.
        3 - If node is a char, print in output char array.
        4 - Repeat the previous steps until boolean array ends.
    */

    int i = 0;
    HuffNode* ptr = root;
    while (i < in.size()) {
        ptr = root;
        int length = 0; // Boolean sequence length to become a char.
        
        while (!ptr->isChar) {

            if (in[i + length] == 1) 
                ptr = ptr->right;
            else 
                ptr = ptr->left;
            
            length++;
        }

        // Special case: file with just one character repeating several times.
        if (length == 0)
            ++i;
        else
            i += length;

        // If HuffNode represents a char, print it in output char array.
        if (ptr->isChar) 
            out.push_back((char)ptr->c);
        else 
            break;
        
    }

    return;
}
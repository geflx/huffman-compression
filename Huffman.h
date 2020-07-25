#ifndef HUFFMAN_H
#define HUFFMAN_H
#include "MyVecNewIterator.h"
#include "MyPriorityQueue.h"
#include <vector>
#include <string>
class HuffNode{
    public:
        /*HuffNode recebe o vetor de frequencias, um char do respectivo caractere, que por padrao  eh: '0'.
        Existe tambem um booleano chamado souUmChar, que indica se o nodo e' ou nao um Nodo que carrega um char ou se ele
        e' um nodo de ligacao.
         */
        HuffNode(int freqs,char c_= '0',bool souUmChar_=false):souUmChar(souUmChar_), freq(freqs),c(c_),right(NULL),left(NULL){};

        int freq;
        char c;
        bool souUmChar;
        HuffNode *right;
        HuffNode *left;
};
class HuffManTree{
    public:
        HuffManTree(int freqs[]);
        HuffManTree(const HuffManTree &obj);
        ~HuffManTree();

        HuffManTree& operator=(const HuffManTree &obj);
        void comprimir(vector<bool> &out, const vector<char> &in) const;
        void descomprimir(vector<char> &out, const vector<bool> &in) const;
    private:
        void preProcessaCompressao();
        void preProcessaAuxiliar(HuffNode *ptr,const string &temp);

        /*Note que o vetor de frequencias contem 257 espacos, isto ocorre pois o ultimo espaco (posicao 256) foi destinada
        a armazenar o numero de bits que foi/sera gravado no arquivo. */
        vector<int> freq=vector<int>(257,0);
        vector<vector<bool>> bits=vector<vector<bool>>(256);
        HuffNode *root;

        HuffNode* copyNodes(const HuffNode *obj);
        void destroy(HuffNode *ptr);
        void create();

};
#endif
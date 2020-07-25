#include "Huffman.h"
#include <iostream>

using namespace std;

HuffManTree::HuffManTree(int freqs[])
{
    root = NULL;
    for (int i = 0; i < 257; i++) //Copiando as frequencias
        freq[i] = freqs[i];

    MyPriorityQueue<HuffNode*> pq;

    for (int j = 0; j < 256; j++) { //Criando os nodos com os caracteres de 0 a 256

        if (freqs[j] > 0) {
            char temporary = j;
            HuffNode* auxiliar = new HuffNode(freqs[j], temporary, true);
            pq.push(auxiliar);
        }

        freq[j] = freqs[j];
    }
    while (pq.size() > 1) { //Realizando o algoritmo de juntar em uma arvore os nodos

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
    root = pq.top(); //Atualizando nossa root
    
    preProcessaCompressao(); //Pre-processa os bits para cada um dos 256 char (se presentes no arquivo)
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

    HuffNode* ans = new HuffNode(ptr->freq, ptr->c, ptr->souUmChar);
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

void HuffManTree::preProcessaCompressao()
{
    /* Funcao importante: Esta funcao armazena os respectivos bits de cada caractere de nossa arvore,
    sua complexidade e' O(HuffNodes) pois percorre todos os nodos da arvore e armazena os vetores de bool. 
    Possui uma funcao auxiliar para sua recursividade, a preProcessaAuxiliar. */
    string temp = "";

    if (root->souUmChar)
        preProcessaAuxiliar(root, temp + "0");
    else 
        preProcessaAuxiliar(root, temp);
    
}

void HuffManTree::preProcessaAuxiliar(HuffNode* ptr, const string& temp)
{
    /* Funcao auxiliar que armazena '1' e '0' nos vetores de bool durante o percurso
    recursivo. */
    if (!ptr)
        return;

    if (ptr->souUmChar) /*Caso eu seja um nodo que armazena um caractere, salvarei o meu percurso. */
        for (int i = 0; i < temp.size(); i++) 

            if (temp[i] == '1') 
                bits[(unsigned char)ptr->c].push_back(true);
            
            else 
                bits[(unsigned char)ptr->c].push_back(false);
            
        
    
    if (ptr->left) /*Caso eu tenha para onde ir, percorrer e levar o '0' comigo */
        preProcessaAuxiliar(ptr->left, temp + "0");
    
    if (ptr->right) /* Levar o 1 comigo */
        preProcessaAuxiliar(ptr->right, temp + "1");
    
}

void HuffManTree::comprimir(vector<bool>& out, const vector<char>& in) const
{
    /* Nesta funcao usaremos nossos bits preProcessados pelas funcoes anteriores,
    percorreremos nosso vetor de Char recebido (in) e adicionaremos os respectivos bits
    de cada letra. */
    for (int i = 0; i < in.size(); i++)
        for (int j = 0; j < bits[(unsigned char)in[i]].size(); j++) 
            if (bits[(unsigned char)in[i]][j]) 
                out.push_back(true);
            else 
                out.push_back(false);
    return;
}

void HuffManTree::descomprimir(vector<char>& out, const vector<bool>& in) const
{
    /* Para descomprimir, realizamos o seguinte algoritmo:
       1- Comece da raiz
       2- Va para direita caso leia '1' ou para esquerda caso '0' ate encontrar um nodo que representa Caractere.
       3- Imprima o caractere no vetor de saida (out)
       4- Repita o processo ate terminar o vetor de booleanos (in)  */
    int contador = 0;
    HuffNode* ptr = root;
    while (contador < in.size()) {
        ptr = root;
        int contaAndamento = 0; // Essa variavel conta o tamanho da "palavra" de bits (necessario para formar um char)
        
        while (!ptr->souUmChar) {

            if (in[contador + contaAndamento] == 1) 
                ptr = ptr->right;
            else 
                ptr = ptr->left;
            
            contaAndamento++;
        }
        /*Caso Especial: Contar Andamento no vetor de bool quando
        so existe um tipo caractere no arquivo (repetido varias vezes) */
        if (contaAndamento == 0)
            ++contador;
        else
            contador += contaAndamento; //Atualiza a "palavra" de bits percorrida no contador total de percorridos.

        if (ptr->souUmChar) // Me encontraram! Printe no arquivo quem eu sou :)
            out.push_back((char)ptr->c);
        else 
            break;
        
    }

    return;
}
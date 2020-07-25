#include "Huffman.h"
#include <fstream>
#include <iostream>
using namespace std;

/* 
    @author = Gabriel Felix
*/

int main(int argc, char** argv)
{

    int frequency[257]; //Possui 257 posicoes pois a ultima foi destinada para o numero de bits impressos no arquivo
    fill(frequency, frequency + 257, 0);

    int numBitsGerados; //Variavel que guardara o numero de bits que gravaremos ou leremos do/no arquivo
    string operation = argv[1];
    string file1 = argv[2]; //Comprimir: comprime file1 e grava em file2
    string file2 = argv[3]; //Descomprimir:  descomprime file1 e grava em file2

    // Compress operation.
    if (operation == "c") { 

        ifstream fonte(file1);
        ofstream saida(file2, ifstream::binary);
        ifstream pointer(file1); //Ponteiro para nos dizer o tamanho do arquivo!
        pointer.seekg(0, pointer.end);
        int length = pointer.tellg();

        // Caso especial: comprimindo um arquivo vazio!
        if (length == 0) {
            saida.write(reinterpret_cast<char*>(frequency), 257 * sizeof(int));
            fonte.close();
            pointer.close();
            return 0;
        }

        vector<char> in(length);
        vector<bool> out;
        char* buffer = new char[length];
        fonte.read(buffer, length); //Lendo de nossa fonte (arquivo que sera comprimido)
        for (int i = 0; i < length; i++) {
            in[i] = buffer[i];
            ++frequency[(unsigned char)in[i]]; //Atualizando as frequencias no vetor frequencia
        }

        HuffManTree arvore(frequency);
        arvore.comprimir(out, in);

        numBitsGerados = out.size(); //Salvando o numero de bitsGerados para imprimir no arquivo.
        delete[] buffer; //Liberando memoria...

        frequency[256] = numBitsGerados; //Passando o numero de bitsGerados para frequencia[256]

        /* Bitwise operations */
        //1- Contando quantas variaveis Char precisaremos para armazenar todos bits
        int contadorChar;
        if (numBitsGerados % 8 == 0) {
            contadorChar = numBitsGerados / 8;
        }
        else {
            contadorChar = numBitsGerados / 8 + 1;
        }

        //2- Alocando o espaco necessario de Char e em seguida armazenando os bits dentro desse vetor.
        char* outTemp = new char[contadorChar];
        fill(outTemp, outTemp + contadorChar, 0);
        int percorreBooleanos = 0;

        for (int i = 0; i < contadorChar; i++) {
            for (int j = 0; j < 8 && percorreBooleanos < numBitsGerados; j++, percorreBooleanos++) { //De 8 em 8 bits salve em cada char
                if (out[percorreBooleanos]) {
                    outTemp[i] |= ((1u) << (7 - j));
                }
            }
        }
        //Comprimimos! Imprima no arquivo:
        saida.write(reinterpret_cast<char*>(frequency), 257 * sizeof(int));
        saida.write(reinterpret_cast<char*>(outTemp), contadorChar * sizeof(char));

        //Liberando memoria.
        fonte.close();
        pointer.close();
        delete[] outTemp;
    }
    // Decompress Operation.
    else if (operation == "d") {
        ifstream fonte(file1, ifstream::binary);
        ofstream saida(file2);

        //Lendo o vetor de frequencias e tambem o numeroBitsGerados que fica na posicao frequency[256]
        fonte.read(reinterpret_cast<char*>(frequency), 257 * sizeof(int));
        numBitsGerados = frequency[256];

        //CASO ESPECIAL: Se nao tem nenhum bitGerado, fim! Nao precisa descomprimir nada. Retorne.
        if (numBitsGerados == 0) {
            saida.close();
            fonte.close();
            return 0;
        }

        //1- Contando quantas variaveis Char precisaremos para armazenar todos bits
        int contadorChar;
        if (numBitsGerados % 8 == 0) {
            contadorChar = numBitsGerados / 8;
        }
        else {
            contadorChar = numBitsGerados / 8 + 1;
        }

        vector<char> out;
        vector<bool> in(numBitsGerados);

        //2- Apontando para posicao inicial de bits com o seekg e em seguida lendo do arquivo.
        char* boolLeitura = new char[contadorChar];
        fonte.seekg(257 * sizeof(int), fonte.beg);
        fonte.read(boolLeitura, contadorChar * sizeof(char));

        //3- Transformando nosso vetor de char que contem booleanos implicitos em um verdadeiro vetor de bool [facilitar].
        int percorreBooleanos = 0;
        int qualPosicaoEstou = 0;
        int contadorVetorIn = 0;
        for (int i = 0; i < contadorChar; i++) {
            for (int j = 0; j < 8 && percorreBooleanos < numBitsGerados; j++, percorreBooleanos++) { //De 8 em 8 bits percorre o char e salve no vetor de bool
                if (boolLeitura[i] & ((1u) << (7 - j))) {
                    in[contadorVetorIn] = true;
                }
                else {
                    in[contadorVetorIn] = false;
                }
                ++contadorVetorIn;
            }
        }
        HuffManTree arvore(frequency); //Crie a arvore e descomprima
        arvore.descomprimir(out, in);

        //Por fim printando o vetor de char no nosso arquivo restaurado
        char* charSaida = new char[out.size()];
        for (int i = 0; i < out.size(); i++)
            charSaida[i] = out[i];
        saida.write(charSaida, out.size());

        //Liberando memoria
        delete[] boolLeitura;
        delete[] charSaida;
        saida.close();
        fonte.close();
    }
}
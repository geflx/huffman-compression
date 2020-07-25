#include "Huffman.h"
#include <fstream>
#include <iostream>

/* 
    @author = Gabriel Felix
    @date = June/2019
    @site = github.com/geflx/huffman-compression
*/

int main(int argc, char** argv)
{

    // Char counter: 256 characters + 1 totalBits = 257 positions.
    int frequency[257]; 
    int totalBits;
    std::fill(frequency, frequency + 257, 0);

    // Command line arguments.
    std::string operation = argv[1];
    std::string file1 = argv[2]; 
    std::string file2 = argv[3]; 

    if (operation == "c") { 

        std::ifstream source(file1);
        std::ofstream output(file2, std::ifstream::binary);

        // Pointer will indicate file size.
        std::ifstream pointer(file1); 
        pointer.seekg(0, pointer.end);
        int length = pointer.tellg();

        // Special case: compressing an empty file.
        if (length == 0) {

            output.write(reinterpret_cast<char*>(frequency), 257 * sizeof(int));

            source.close();
            pointer.close();

            return 0;
        }

        // Reading data from input file.
        std::vector<char> in(length);
        char* buffer = new char[length];
        source.read(buffer, length); 

        std::vector<bool> out;

        for (int i = 0; i < length; i++) {

            in[i] = buffer[i];
            ++frequency[(unsigned char)in[i]]; // Update char frequency.

        }

        HuffManTree my_tree(frequency);
        my_tree.compress(out, in);

        // Bit counter will be saved in position[257].
        totalBits = out.size(); 
        frequency[256] = totalBits; 
        delete[] buffer; 


        // Counting how many chars will be needed to store all data.
        int totalChars;

        if (totalBits % 8 == 0) 
            totalChars = totalBits / 8;
        else 
            totalChars = totalBits / 8 + 1;
        

        // Alocating and storaging data in output array.
        char* outTemp = new char[totalChars];
        std::fill(outTemp, outTemp + totalChars, 0);

        // Bitwise operations over chars to storage data.
        int aux = 0;
        for (int i = 0; i < totalChars; i++) 
            for (int j = 0; j < 8 && aux < totalBits; j++, aux++)  
                // Each char can represent 8 bits.
                if (out[aux]) 
                    outTemp[i] |= ((1u) << (7 - j));
                
        // Printing in file...
        output.write(reinterpret_cast<char*>(frequency), 257 * sizeof(int));
        output.write(reinterpret_cast<char*>(outTemp), totalChars * sizeof(char));

        // Freeing memory.
        source.close();
        pointer.close();
        delete[] outTemp;
    }
    else if (operation == "d") {

        std::ifstream source(file1, std::ifstream::binary);
        std::ofstream output(file2);

        // Reading char's frequency array and totalBits. 
        source.read(reinterpret_cast<char*>(frequency), 257 * sizeof(int));
        totalBits = frequency[256];

        // Special case: decompressing an empty file. Return.
        if (totalBits == 0) {

            output.close();
            source.close();

            return 0;
        }

        // Counting how many chars will be needed to represent all binaries.
        int totalChars;
        if (totalBits % 8 == 0) 
            totalChars = totalBits / 8;
        else 
            totalChars = totalBits / 8 + 1;
        
        std::vector<char> out;
        std::vector<bool> in(totalBits);

        // Pointing to file begin and reading data from it.
        char* inputBooleans = new char[totalChars];
        source.seekg(257 * sizeof(int), source.beg);
        source.read(inputBooleans, totalChars * sizeof(char));

        // Transforming implicit data inside chars into a real boolean array.
        int aux = 0;
        int inputPos = 0;

        for (int i = 0; i < totalChars; i++) {
            for (int j = 0; j < 8 && aux < totalBits; j++, aux++) { 

                // Read the 8 bits inside a char and create an 8-bit boolean array.
                if (inputBooleans[i] & ((1u) << (7 - j))) 
                    in[inputPos] = true;
                else 
                    in[inputPos] = false;
                
                ++inputPos;
            }
        }

        // Create huffman tree and decompress.
        HuffManTree my_tree(frequency); //Crie a my_tree e descomprima
        my_tree.decompress(out, in);

        // Printing answer's char array in output file. 
        char* outputChar = new char[out.size()];
        for (int i = 0; i < out.size(); i++)
            outputChar[i] = out[i];
        output.write(outputChar, out.size());

        //Freeing memory.
        delete[] inputBooleans;
        delete[] outputChar;

        // Closing files.
        output.close();
        source.close();
    }

    return 0;
}
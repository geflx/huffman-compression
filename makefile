all: HuffManTree

HuffManTree: Huffman.o main.o
	g++ -g -o main Huffman.o main.o -O3
Huffman.o: Huffman.cpp Huffman.h
	g++ -g -c Huffman.cpp
main.o: main.cpp Huffman.h
	g++ -g -c main.cpp
clear:
	rm *.o
	rm main

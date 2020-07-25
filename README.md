# Huffman Tree :deciduous_tree: File Compression 

An application to easily and optimally (proved) compress files **without losing any data**.  
This application can be very **fast** and **reduce file size** when the input file contains a lot of repeated characters.

## How to run?

1- Make sure g++ is installed in your computer.
2- Type the following command to compile:
```
$ make
```
3- To **compress** a file "myFile.extension", input the following command:
```
./a.out compress myFile.extension compressed.huff
```
4- To **decompress** a file "compressed.huff", now type:
```
./a.out decompress compressed.huff myFile.extension 
```

Now, your file "myFile.extension" is ready to go!

## How does it work?

A file is simply an amount of characters, so we will how many times each character shows up and **codify** each one with "0"s and "1"s.  
In this implementation, the variable encoding is used (for a code _i_, **there isn't** any other code where _i_ is and preffix).  
For instance, the file "letters.txt" contains the following data:
```
ABBBBBBBBACD
```
Notice that "B" is by far the most repeated character in file (8x), followed by "A" (2x), C and D (1x).  
So we will codify B as "0", A as "10", C as "110" and D as "111".  
The main idea is to use **represent with less bits** the most repeated characters, to **economize space**. The example diagram is shown below:  

<p align="center">
	<td><img src="assets/example.png" width=150 height=220></td>
</p>

After optimally codify the characters, it is easy to decodify the file using a "master" array which tell us which boolean array represents each character.

## Conclusions

The development of this application was very important to understand better about file representatation and also improve my tecniques about file handling.
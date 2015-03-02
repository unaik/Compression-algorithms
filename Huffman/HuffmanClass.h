#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>

using std::cout;
using std::cin;
using std::endl;


class Huffman
{
private:
	Huffman();
	Huffman(const Huffman&);
	Huffman& operator=(const Huffman&);
	char *filename,*outputfile,*compressedfilena;
	

public:
	std::map<char,char*> hashtable;
	Huffman(char* file,char* outp,char *comp);	
	void dohuff();
	
};
#endif
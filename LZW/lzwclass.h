#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>


class LZW
{
private:
	char *filename,*outputfilename;
	
	LZW(); //default constructor
	LZW(const LZW&); //copy constructor
	LZW& operator=(const LZW&); //assignment operator

public:
	LZW(char* name,char* opname);
	void compress();
	void decompress();
};
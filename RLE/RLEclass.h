#ifndef RLE_H
#define RLE_H

#include <iostream>
#include <cstdlib>
#include <cstdio>


class RLE
{
public:
	RLE(char* name,char* opname);
	void getname();
	void compress();
	void decompress();

private:
	char* filename;
	char* outputfile;
	char runcount;
	
	RLE(); //default constructor
	RLE(const RLE&); //copy constructor.
	RLE& operator=(const RLE&); //assignment operator
};


#endif
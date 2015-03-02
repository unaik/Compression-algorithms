#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "HuffmanClass.h"

int main()
{
	char filename[50] ,outputfile[50] ,comp[50];
	cout<<"Enter the input file "<<endl;
	cin>>filename;
	cout<<endl;
	cout<<"Compressed file name "<<endl;
	cin>>comp;
	cout<<endl;
	cout<<"Outputfile name "<<endl;
	cin>>outputfile;
	cout<<endl;
	Huffman huffm(filename,outputfile,comp);
	huffm.dohuff();
}
#include "rle.h"

using std::cout;using std::endl;

int main()
{
	char name[50] ,opname[50] ;
	std::cout<<"Enter the name of the input file :"<<std::endl;
	std::cin>>name;
	
	std::cout<<"Enter the name of the output file :"<<std::endl;
	std::cin>>opname;


	RLE rle(name,opname);
	rle.compress();
	rle.decompress();

	return 0;
	
}

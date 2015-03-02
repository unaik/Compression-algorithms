#include "lzwclass.h"

#define no 65536

using std::cout;using std::endl;

LZW::LZW(char* name,char* opname) :
filename(name),
outputfilename(opname)
{
	std::cout<<"LZW class has been initialized"<<std::endl;
	std::cout<<"The compressed file is named : lcomp.txt "<<std::endl<<std::endl;
	std::cout<<"filename : "<<filename<<std::endl;
	std::cout<<"outputfilename : "<<outputfilename<<std::endl;
}

void DictionarySearch(unsigned char** dictionary,std::vector<int>& patlength,const int& dictsize,unsigned char * p, const unsigned short int& psize,bool& indicator)
{
	 for(int i=0;i<dictsize;i++)
	 {
		 if(psize == patlength[i])
		 {
			 indicator = true;
			 for(int j=0; j<patlength[i];j++ )
			 {
				 if(p[j] == dictionary[i][j])
				 {
					 indicator &= indicator;
				 }

				 if(p[j] != dictionary[i][j])
				 {
					 indicator = false;
				 }
			 }
		 }

		 if(indicator == true)
		 {break;}

	 }
}

void WSearch(unsigned char** dictionary, unsigned char *p, std::vector<int>& patlen, const unsigned short& psize, const int& iterno , bool& indicator,const bool& type)
{
	
	
	if(type == true)
	{
		if(psize-1 == patlen[iterno])
		{
			indicator == true;
			for(int i=0;i<patlen[iterno];i++)
			{
				if(p[i] == dictionary[iterno][i])
				{
					indicator &= indicator;
				}

				if(p[i] != dictionary[iterno][i])
				{
					indicator = false;
				}
			}
		}
	}

	if(type == false)
	{
		if(psize == patlen[iterno])
		{
			indicator == true;
			for(int i=0;i<patlen[iterno];i++)
			{
				if(p[i] == dictionary[iterno][i])
				{
					indicator &= indicator;
				}

				if(p[i] != dictionary[iterno][i])
				{
					indicator = false;
				}
			}
		}
	}
}



void Initroots(unsigned char** dictionary,char* filename,std::vector<int>& patlength,int& size)
{
	FILE *fpt;
	unsigned char charac;
	int count=0;
	bool control = true;
	fpt = fopen(filename,"rb");
	
	int i;
	i=300;
	while(fread(&charac,sizeof(unsigned char),1,fpt) == 1)
	{
		if(count == 0 || count ==1)
		{
			dictionary[count]= new unsigned char;
			dictionary[count][0] = charac; 
			count++;
			size+=1;
		}

		if(count >1)
		{
			for(int i=1; i<count;i++)
			{
				if(charac == dictionary[i][0])
				{
					control = false;
					continue;
				}
			}
			
			if(control == false){control = true;continue;}

			if(control == true)
			{
				dictionary[count] = new unsigned char;
				dictionary[count][0] = charac;
				patlength[count] = 1;
				size+=1;
				count++;
			}
		}
	}
	cout<<"this seems to be working"<<endl;
	fclose(fpt);
}

void Initroots1(unsigned char** dictionary,std::vector<int>& patlength,int& size)
{
	for(int count=0;count<256;count++)
	{
		dictionary[count] =new unsigned char;
		dictionary[count][0] = count;
		patlength[count] = 1;
		size +=1;
	}

}



void LZW::compress()
{
	unsigned char *dictionary[no], *P;
	std::vector<int>patternlen(no);
	int dictsize = 0;
	bool match =false,endf = false,op =false;
	unsigned short int psize = 0;

	
	Initroots1(dictionary,patternlen,dictsize);

	
	FILE  *inputf,*intermf,*outputf;
	unsigned char C;
	inputf = fopen(filename,"rb");
	intermf = fopen("lcompress.txt","wb");

	while(fread(&C,sizeof(unsigned char),1,inputf)==1)
	{
		if(psize == 0)
		{
			P = new unsigned char;
			P[psize] = C;
			psize++;
		}
		else if(psize > 0)
		{
			P = (unsigned char*)realloc(P,psize*sizeof(psize) + 1);
			P[psize] = C;
			psize++;
		}

		DictionarySearch(dictionary,patternlen,dictsize,P,psize,match);
		
		bool type;
		type = true;

		if(match == true)
		{endf == true;match =false;}
				
		else
		{
			for(int i=0;i<dictsize;i++)
			{
				WSearch(dictionary,P,patternlen,psize,i,op,type);
				if((psize-1) == patternlen[i])
				{
					op = true;
					for(int j=0;j<patternlen[i];j++)
					{
						if(P[j] == dictionary[i][j])
						{op &= op;}

						if(P[j] != dictionary[i][j])
						{op = false;}
					}
				}
				if(op == true)
				{
					op = false;
					fwrite(&i,sizeof(unsigned short int),1,intermf);
					break;
				}
			}

			dictionary[dictsize] = new unsigned char[psize];
			
			for(int i=0; i<psize;i++)
			{dictionary[dictsize][i] = P[i];}
			patternlen[dictsize] = psize; 
			dictsize +=1;

			P = (unsigned char*)realloc(P,sizeof(unsigned char));
			P[0] = C;
			psize = 1;
			endf =false;
		}
	}

	bool type2;
	type2 = false;

	if(endf == true)
	{
		for(int i=0;i<dictsize;i++)
		{
			WSearch(dictionary,P,patternlen,psize,i,op,type2);
			if((psize) == patternlen[i])
				{
					op = true;
					for(int j=0;j<patternlen[i];j++)
					{
						if(P[j] == dictionary[i][j])
						{op &= op;}

						if(P[j] != dictionary[i][j])
						{op = false;}
					}
				}
			if(op == true)
			{
				fwrite(&i,sizeof(unsigned short int),1,intermf);
				op = false;
				break;
			}
		}
		endf = false;
	}

	else if(endf == false)
	{
		for(int i=0;i<dictsize;i++)
		{
			WSearch(dictionary,P,patternlen,psize,i,op,type2);
			if((psize-1) == patternlen[i])
				{
					op = true;
					for(int j=0;j<patternlen[i];j++)
					{
						if(P[j] == dictionary[i][j])
						{op &= op;}

						if(P[j] != dictionary[i][j])
						{op = false;}
					}
				}
			if(op == true)
			{
				fwrite(&i,sizeof(unsigned short int),1,intermf);
				op = false;
				break;
			}
		}
	}
	
	
	/*delete [] P;
	fclose(inputf);
	fclose(intermf);*/
}








void LZW::decompress()
{
	FILE *inter,*fin;
	unsigned short C,P;
	unsigned char* dictionary[no],write;
	unsigned char *a=new unsigned char,*b=new unsigned char;
	int patternlen[no];
	int dictsize=0;
	bool match=false;

	
	for(int i=0;i<256;i++)
	{
		dictionary[i] = new unsigned char;
		dictionary[i][0] =(int)(i);
		dictsize +=1;
		patternlen[i] = 1;
	}

	inter = fopen("lcompress.txt","rb");
	fin = fopen(outputfilename,"wb");

	fread(&C,sizeof(unsigned short),1,inter);
	for(int i=0;i<dictsize;i++)
	{
		if(i==C)
		{
			for(int j=0;j<patternlen[i];j++)
			{
				write = dictionary[i][j];
				fwrite(&write,sizeof(unsigned char),1,fin);
			}
			
		}
	}
	P = C;
	while(fread(&C,sizeof(unsigned short),1,inter) == 1)
	{
		
		for(int i=0;i<dictsize;i++)
		{
			if(i == C)
			{
				match = true;
				
				for(int j=0;j<patternlen[i];j++)
				{
					write = dictionary[i][j];
					fwrite(&write,sizeof(unsigned char),1,fin);
				}
				
				a = (unsigned char*)realloc(a,sizeof(unsigned char)*patternlen[P]);
				
				for(int j=0;j<patternlen[P];j++)
				{a[j] = dictionary[P][j];}

				b = (unsigned char*)realloc(b,sizeof(unsigned char));
				b[0] = dictionary[C][0];

				
				dictionary[dictsize] = new unsigned char[P+1];
				for(int j=0;j<patternlen[P];j++)
				{dictionary[dictsize][j] = a[j];}
				dictionary[dictsize][patternlen[P]] = b[0];
				patternlen[dictsize] = patternlen[P]+1;
				dictsize = dictsize+1;
				break;
			}

			if(i!=C)
			{match=false;}
		}

		if(match == true)
		{match == false;}

		else
		{
			a = (unsigned char*)realloc(a,sizeof(unsigned char)*patternlen[P]);
			for(int i=0;i<patternlen[P];i++)
			{
				a[i] = dictionary[P][i];
				fwrite(&dictionary[P][i],sizeof(unsigned char),1,fin);
			}

			b = (unsigned char*)realloc(b,sizeof(unsigned char));
			b[0] = dictionary[P][0];
			fwrite(&dictionary[P][0],sizeof(unsigned char),1,fin);
			dictionary[dictsize] = new unsigned char[P+1];
			for(int i=0;i<patternlen[P];i++)
			{dictionary[dictsize][i] = a[i];}

			dictionary[dictsize][patternlen[P]] = b[0];
			patternlen[dictsize] = patternlen[P]+1;
			dictsize+=1;
		}

		P = C;
	}
}
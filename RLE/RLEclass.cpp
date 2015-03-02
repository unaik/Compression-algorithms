#include "RLEclass.h"

using std::cout;using std::endl;using std::cin;

RLE::RLE(char* name,char* opname) : 
filename(name),
outputfile(opname)
{
cout<<"RLE class has been initialized"<<endl;
cout<<filename<<endl<<outputfile<<endl;
}


void RLE::compress()
{
	FILE *inputf,*intermf;
	unsigned char A,B,C,marker='$';
	int runcount =0;
	runcount = 0;
	long filesize;
	unsigned char* buffer;
	fpos_t position;
	bool compression = true;

	cout<<"filename is == "<<filename<<endl;

	inputf = fopen(filename,"rb");
	intermf = fopen("RLEc.txt","wb");

	fseek (inputf , 0 , SEEK_END);
    filesize = ftell (inputf);
	rewind (inputf);

	
	buffer = new unsigned char[filesize];
	int i=0;
	while(fread(&C,sizeof(unsigned char),1,inputf) == 1)
	{
		buffer[i] = C;
		i++;
	}

	A= buffer[0];

	for(int i=0;i<filesize;i++)
	{
		B = buffer[i];
		if(B == A)
		{
			if(runcount ==254)
			{
				if(compression == false)
				{
					fwrite(&marker,sizeof(unsigned char),1,intermf);
					fwrite(&marker,sizeof(unsigned char),1,intermf);
					fwrite(&runcount,sizeof(unsigned char),1,intermf);
					fwrite(&A,sizeof(unsigned char),1,intermf);
					compression = true;
					runcount = 1;
					A = B;
				}

				else 
				{
					fwrite(&runcount,sizeof(unsigned char),1,intermf);
					fwrite(&A,sizeof(unsigned char),1,intermf);
					runcount = 1;
					A = B;
				}
			}

			else 
			{
				runcount += 1;
			}
		}

		else
		{
			if(buffer[i] == marker )
			{
				if(compression == false)
					{
						fwrite(&marker,sizeof(unsigned char),1,intermf);
						fwrite(&marker,sizeof(unsigned char),1,intermf);
						fwrite(&runcount,sizeof(unsigned char),1,intermf);
						fwrite(&A,sizeof(unsigned char),1,intermf);
						A = B;
						runcount = 1;
						compression = true;
					}

					else
					{
						fwrite(&runcount,sizeof(unsigned char),1,intermf);
						fwrite(&A,sizeof(unsigned char),1,intermf);
						A = B;
						runcount;
					}
			}

			else
			{
				if(A == marker)
				{
					if(compression == false)
					{
						fwrite(&marker,sizeof(unsigned char),1,intermf);
						fwrite(&marker,sizeof(unsigned char),1,intermf);
						fwrite(&runcount,sizeof(unsigned char),1,intermf);
						fwrite(&A,sizeof(unsigned char),1,intermf);
						A = B;
						runcount = 1;
						compression = true;
					}

					else
					{
						fwrite(&runcount,sizeof(unsigned char),1,intermf);
						fwrite(&A,sizeof(unsigned char),1,intermf);
						A = B;
						runcount = 1;
					}
				}

				else
				{
					
						if(compression == false)
						{
							fwrite(&marker,sizeof(unsigned char),1,intermf);
							fwrite(&marker,sizeof(unsigned char),1,intermf);
							fwrite(&runcount,sizeof(unsigned char),1,intermf);
							fwrite(&A,sizeof(unsigned char),1,intermf);
							A = B;
							runcount = 1;
							compression = true;
						}

						else
						{
							fwrite(&runcount,sizeof(unsigned char),1,intermf);
							fwrite(&A,sizeof(unsigned char),1,intermf);
							A = B;
							runcount = 1;
						}
				}
			}
		}
	}
		
	if(A == marker)
	{
		if(compression == false)
		{
			fwrite(&marker,sizeof(unsigned char),1,intermf);
			fwrite(&marker,sizeof(unsigned char),1,intermf);
			fwrite(&runcount,sizeof(unsigned char),1,intermf);
			fwrite(&A,sizeof(unsigned char),1,intermf);
			A = B;
			runcount = 1;
			compression = true;
		}

		else
		{
			fwrite(&runcount,sizeof(unsigned char),1,intermf);
			fwrite(&A,sizeof(unsigned char),1,intermf);
			A = B;
			runcount = 1;
		}
	}

	else
	{
			if(compression == false)
			{
				fwrite(&marker,sizeof(unsigned char),1,intermf);
				fwrite(&marker,sizeof(unsigned char),1,intermf);
				fwrite(&runcount,sizeof(unsigned char),1,intermf);
				fwrite(&A,sizeof(unsigned char),1,intermf);
				runcount = 1;
			}

			else
			{
				fwrite(&runcount,sizeof(unsigned char),1,intermf);
				fwrite(&A,sizeof(unsigned char),1,intermf);
				runcount = 1;
			}
	}
}


void RLE::decompress()
{
	FILE *intermf,*outputf;
	unsigned char runcount =0;
	unsigned char a,b,c;
	unsigned char marker ='$';
	bool markerind = false;
	unsigned char * buffer;
	long filesize;

	intermf = fopen("RLEc.txt","rb");
	outputf = fopen(outputfile,"wb");

	fseek (intermf , 0 , SEEK_END);
    filesize = ftell (intermf);
	rewind (intermf);

	buffer = new unsigned char[filesize];
	int i=0;
	while(fread(&c,sizeof(unsigned char),1,intermf) == 1)
	{
		buffer[i] = c;
		i++;
	}
	rewind(intermf);

	while(fread(&c,sizeof(unsigned char),1,intermf) == 1)
	{
		if(c!=marker)
		{
			runcount = c;
			fread(&c,sizeof(unsigned char),1,intermf);
			for(int i=1;i<=runcount;i++)
			{
				fwrite(&c,sizeof(unsigned char),1,outputf);
			}
		}

		else
		{
			fread(&c,sizeof(unsigned char),1,intermf);
			if(c == marker)
			{
				if(markerind == true)
				{
					markerind = false;
					fread(&c,sizeof(unsigned char),1,intermf);
					while(c != marker)
					{
						fwrite(&c,sizeof(unsigned char),1,outputf);
						fread(&c,sizeof(unsigned char),1,intermf);
					}
					fseek(intermf,-sizeof(unsigned char),SEEK_CUR);
				}
			
				else
				{
					markerind = false;
				}
			}

			else if (c != marker)
			{
				fseek(intermf,-sizeof(unsigned char),SEEK_CUR);
				fseek(intermf,-sizeof(unsigned char),SEEK_CUR);
				fread(&c,sizeof(unsigned char),1,intermf);
				runcount = c;
				fread(&c,sizeof(unsigned char),1,intermf);
				for(int i=1;i<=runcount;i++)
				{fwrite(&c,sizeof(unsigned char),1,outputf);}
			}
		}
	}

}


	


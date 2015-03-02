#include "HuffmanClass.h"

struct characters
{
	unsigned char symbol;
	int freq;
	bool write;

	bool operator< (const characters& other)
	{return freq<other.freq;}
};

struct node
{
	unsigned char symbol;
	int frequency;
	node *left;
	node *right ;
	node *parent;

	bool operator< (const node& other)
	{return frequency<other.frequency;}
};

bool comp(const node &a,const node &b)
{
	if(a.frequency<b.frequency)
	{return true;}
	else
	{return false;}
}


std::vector<node> Createtree(std::vector<node> leaf,int treesize)
{
	node placeholder;
		
	std::vector<node> tree;
	int count = 0;
	while(treesize-1>0)
	{
		count++;
		
		placeholder.frequency = leaf[0].frequency+leaf[1].frequency;
		placeholder.symbol = -1;
	
		placeholder.right = new node;
		(placeholder.right)->frequency = leaf[1].frequency;
		(placeholder.right)->left = leaf[1].left;
		(placeholder.right)->right = leaf[1].right;
		(placeholder.right)->parent = NULL;
		(placeholder.right)->symbol = leaf[1].symbol;
		
		placeholder.left = new node;
		(placeholder.left)->frequency = leaf[0].frequency;
		(placeholder.left)->left = leaf[0].left;
		(placeholder.left)->right = leaf[0].right;
		(placeholder.left)->parent = NULL;
		(placeholder.left)->symbol = leaf[0].symbol;
				
		leaf[0].frequency = placeholder.frequency;
		leaf[0].left = placeholder.left;
		leaf[0].right = placeholder.right;
		leaf[0].symbol = placeholder.symbol;
		//leaf[0].parent = placeholder.parent;

		for(int j=1;j<treesize;j++)
		{leaf[j] = leaf[j+1];}

		std::sort(leaf.begin(),leaf.begin()+(treesize-1));
		treesize -= 1;
	}
	
	tree.resize(leaf.size());
	for(int i=0;i<leaf.size();i++)
	{tree[i] = leaf[i];}

	return tree;
}

void patterngen(int tree_height,char *position,node *tree,char **hashtable)
{	

	if((tree->left != NULL) || (tree->right != NULL))
	{	position[tree_height]='0';
		patterngen(tree_height+1,position,(*tree).left,hashtable);
		position[tree_height]='1';
		patterngen(tree_height+1,position,(*tree).right,hashtable);
	}

	else
	{	position[tree_height]=0;
		hashtable[tree->symbol] = new char[sizeof(position) + 1];
		std::strcpy(hashtable[tree->symbol],position);
	}
}

void bitpatterngen(const std::vector<node>& tree,int index,char *position,char **pattern)
{
	cout<<"creating bitpatterns "<<endl;
	node *treetemp;
	treetemp = new node[tree.size()];
	for(unsigned int i=0;i<tree.size();i++)
	{treetemp[i] = tree[i];}
	patterngen(0,position,treetemp,pattern);
	delete [] treetemp;
}



void Compression(char **dictionary,unsigned char* buffer,char *compressedfilename,int lsize,std::vector<characters>& charac,int& rem,int& writ,int& indic)
{
	
	char comp =0;
	FILE *compress;
	compress = fopen(compressedfilename,"wb");
		
	int csize = charac.size();
	fwrite(&csize,sizeof(int),1,compress);
	for(int i=0;i<lsize;i++)
	{
		for(int j=0;j<charac.size();j++)
		{
			if(buffer[i] == charac[j].symbol)
			{
				if(charac[j].write == true)
				{
					fwrite(&charac[j].symbol,sizeof(unsigned char),1,compress);
					fwrite(&charac[j].freq,sizeof(int),1,compress);
					charac[j].write = false;
				}
			}
		}
	}

	for(int i=0;i<lsize;i++)
	{
		for(char* j=dictionary[buffer[i]];*j!=NULL;j++)
		{
			comp = comp*2;
			if(*j == '1')
			{comp =comp|1;}
			rem+=1;

			if(rem == 8)
			{
				fwrite(&comp,sizeof(char),1,compress);
				rem=0;
				comp=0;
				writ+=1;
			}
		}
	}
	int c = 8-rem;
	if(rem>0 && rem<8)
	{
		for(int i=0;i<c;i++)
		{
			comp *=2;
		}
		indic =1;
		fwrite(&comp,sizeof(char),1,compress);
	}
	
	fclose(compress);
}


void dec(char * compressedfilename,char *outputfilename,char **pattern, std::vector<characters>& dictionary, int rem, int writ,int indic)
{
	char charac;int charac1;
	char temp;char *str1,*str2;
	str1 = new char;
	int counter=0;
	FILE *compressed,*output;
	compressed=fopen(compressedfilename,"rb");
	output = fopen(outputfilename,"wb");
	fread(&charac1,sizeof(int),1,compressed);
	for(int i=0;i<dictionary.size();i++)
	{
		fread(&charac,sizeof(char),1,compressed);
		fread(&charac1,sizeof(int),1,compressed);
	}
	int k=0;bool success = false;
	while(fread(&charac,sizeof(char),1,compressed) == 1)
	{
		
		int j=0,i=7;
		str2 = new char;

		while(i>=0)
		{
		
			temp = charac;
			int temp1 = pow(2,i);
			temp &=  temp1;

			if(temp == 0)
			{str2[j] = '0';}
			else
			{str2[j] = '1';}

			str2[j+1] = NULL;
			str1[k] = str2[j];
			str1[k+1] = NULL;

			for(int l=0;l<256;l++)
			{
				if(strcmp(str1,pattern[l])==0)
				{
					success = true;
					fwrite(&l,sizeof(unsigned char),1,output);
					break;
				}
			}

			if(success == false)
			{k++;}
			
			else
			{str1 = new char;k=0;success = false;}
					
			j+=1;
			i-=1;
		}
	counter+=1;	
	if(counter == writ)
		{
			if(indic == 1)
			{
				fread(&charac,sizeof(char),1,compressed);
				int i=7,k=0,tt = 7-rem;
				while(i>tt)
				{
					temp = charac;
					int temp1 = pow(2,i);
					temp &= temp1;
	
					if(temp == 0)
					{str2[k] = '0';}
					else
					{str2[k] = '1';}
					str2[k+1] = NULL;

					for(int l=0;l<256;l++)
					{
						if(strcmp(str2,pattern[l])==0)
						{
							success = true;
							fwrite(&l,sizeof(char),1,output);
							break;
						}
					}

					if(success == false)
					{
						k++;
					}

					else
					{str2=new char;k=0;success=false;}
					i-=1;
				}
			}
			break;
		}	
	}
	
	cout<<" counter = "<<counter<<endl;
	
}



Huffman::Huffman(char *file,char *outp,char *comp):
	filename(file),
	outputfile(outp),
	compressedfilena(comp),
	hashtable()
{
	cout<<"Filename = "<<filename<<endl;
	cout<<"Outputfile name = "<<outputfile<<endl;
	cout<<"Compressedfile = "<<compressedfilena<<endl;
	cout<<"Huffman class has been initialized"<<endl;
}

void Huffman::dohuff()
{
	FILE *infile,*compressedfile,*outfile;
	long lsize;
	int count = 1;
	int height = 0;
	unsigned char *buffer;
	
	char *position;
	char **pattern;
	position = new char[256];
	pattern = new char*[256];
	for(int i=0;i<256;i++)
	{pattern[i] = new char();}
	
	std::vector<characters> character;
	unsigned char curr,prev;
	std::vector<unsigned char>temp;
	std::vector<int> thecount[256];
	std::vector<unsigned char>::iterator iter;
	bool first = true,flag =false;

	
	infile = fopen(filename,"rb");
	

	fseek(infile,0,SEEK_END);
	lsize = ftell(infile);
	rewind(infile);

	buffer = new unsigned char[lsize];
	fread(buffer,sizeof(unsigned char),lsize,infile);

	
	int test[256];
	for(int i=0;i<256;i++)
	{test[i]=0;}

	for(int i=0;i<lsize;i++)
	{
		test[buffer[i]] += 1;
	}

	int coun =0;
	for(int i=0;i<256;i++)
	{
		characters tempo;
		if(test[i]>0)
		{
			tempo.symbol = i;
			tempo.freq = test[i];
			tempo.write = true;
			character.push_back(tempo);
		}
	}

	std::vector<bool> writperm (256,true);
	std::vector<characters> newchar;
	

	for(int i=0;i<lsize;i++)
	{
		characters tempo;
		tempo.symbol = buffer[i];
		tempo.freq = -1;
		if(writperm[buffer[i]] == true)
		{
			newchar.push_back(tempo);
		}
		writperm[buffer[i]] = false;
	}
	
	for(unsigned int i=0;i<newchar.size();i++)
	{
		for(unsigned int j=0;j<character.size();j++)
		{
			if(newchar[i].symbol == character[j].symbol)
			{
				newchar[i].freq = character[j].freq;
			}
		}
	}

	int csize = character.size();
	int treesize = character.size();


	for(int i=0;i<character.size();i++)
	{character[i].write = true;}
	
	std::vector<node> leaf;
	std::vector<node> tree;
	node leaftemp;
	
	for(int i=0;i<treesize;i++)
	{
		leaftemp.symbol = newchar[i].symbol;
		leaftemp.frequency =newchar[i].freq;
		leaftemp.parent = NULL;
		leaftemp.left = NULL;
		leaftemp.right = NULL;
		leaf.push_back(leaftemp);
	}

	leaf.resize(256);
	
	std::sort(leaf.begin(),leaf.begin()+treesize);
	tree = Createtree(leaf,treesize);
	
	
	node *treenew;

	treenew = new node[tree.size()];
	for(int i=0;i<tree.size();i++)
	{treenew[i] = tree[i]; }

	bitpatterngen(tree,0,position,pattern);
		
	for(int i=0;i<256;i++)
	{
		hashtable[i] = pattern[i];
	}

	

	for(int i=0;i<256;i++)
	{
		if(hashtable[(char)i][0]!=NULL)
		{cout<<(char)i<<" "<<hashtable[(char)i]<<endl;}
	}

	int remain=0,written=0,indic=0;

	Compression(pattern,buffer,compressedfilena,lsize,character,remain,written,indic);
	
	cout<<"r w i"<<" "<<remain<<" "<<written<<" "<<indic<<endl;
	system("pause");
	dec(compressedfilena,outputfile,pattern,character,remain,written,indic);
	system("pause");
		


	delete[] treenew;
	delete[] buffer;
	
	
}





#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>   
#include <fcntl.h>
#include <fstream>
#include <sys/types.h>   
#include <sys/stat.h>   
#include <fcntl.h>  
#include <time.h>
using namespace std;
struct resources
{
	string path;
	double similarly;
};
resources r[6005];
int in = 0;
string pathupload;
string pathlocal;
string pathoutput;
string path_of_resources;
clock_t start, finish; 
float comp(vector<float> a, vector<float>b)
{
	float sum = 0;
	for (int i = 0; i < a.size(); i++)
	{
		if(a[i]>b[i])
			sum += a[i]-b[i];
		else
			sum += b[i]-a[i];
		
	}
	return sum;
}
char* ReadSpeacialLine(char* filename, int whichLine)
{
	if (whichLine < 0 || NULL == filename)
	{
		return NULL;
	}
	FILE *fp = fopen(filename, "r");
	if (NULL == fp) {
		return NULL;
	}
	int reachWhichLine = 0;
	int curLine = 1;
	#define LINE_SIZE 10000  
	char *data = NULL;
	data = (char*)malloc(LINE_SIZE);
	while (!feof(fp))
	{
		memset(data, 0, LINE_SIZE);
		fgets(data, LINE_SIZE - 1, fp);
		curLine++;
		if (curLine > whichLine)
		{
			reachWhichLine = 1; 
			break;
		}
	}
	fclose(fp);
		
	return 0 == reachWhichLine ? NULL : data;
	free(data);
	data = NULL;
}

int getNumberOfEdges(string fileName)
{
	char flag;
	int edgeNum;
	int count=0;
	FILE *fp = fopen(fileName.c_str(), "rt+");//open the TXT file, can only read, cannot write  
	while (!feof(fp))
	{
		flag = fgetc(fp);
		if (flag == '\n') count++;
	}
	edgeNum = count + 1;
	fclose(fp);
	return edgeNum - 1;
}

string& replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}
void sort()
{
	for (int i = 0; i < in ; i++)
	{

		for (int j = i; j < in; j++)
		{
			if (r[i].similarly > r[j].similarly)
			{
				resources t;
				t.path = r[i].path;
				t.similarly = r[i].similarly;
				r[i].path = r[j].path;
				r[i].similarly = r[j].similarly;
				r[j].path = t.path;
				r[j].similarly = t.similarly;
			}
		}

	}
}
int readFileList(char *basePath, char* classes)
{
	start = clock(); 
	DIR *dir;
	struct dirent *ptr;
	char base[6005];
	if ((dir = opendir(basePath)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		else if (ptr->d_type == 8)    ///file
		{
			string cla = classes;
			char str3[200];
			string copy = basePath;
			copy.append("/");
			strcpy(str3, copy.c_str());
			strcat(str3, ptr->d_name);
			cout<<"str3:"<<str3<<endl;
			string full_path = str3;
			string videofullpath = full_path;
			
			if (!cla.compare("video"))
			{
				int l1 = getNumberOfEdges(pathupload);
				pathlocal = full_path;
				int l2 = getNumberOfEdges(pathlocal);
				
				const char* expru = pathlocal.c_str();
				char *bufu = new char[strlen(expru) + 1];
				strcpy(bufu, expru);
				char* pathofvideo = ReadSpeacialLine(bufu, 1);
				pathofvideo[strlen(pathofvideo)-1]=0;
				std::ifstream infileupload;
				infileupload.open(pathupload.c_str());
				string lineupload;
				float min_all=0;
				while(getline(infileupload,lineupload))
				{
					vector<float> f1;
					//read cur line of file upload
					stringstream b;
					b.str(lineupload);					
					while (b)
					{
						float c;
						b >> c;
						f1.push_back(c);
					}	
					float min=0;
					float re = 0;
					int i=0;
					std::ifstream infile;
					infile.open(pathlocal.c_str());
					string line;
					while(getline(infile,line))
					{
						i++;
						// get the line of local
						stringstream a;
						a.str(line);	
						vector<float> f2;						
						while (a)
						{
							float c;
							a >> c;
							f2.push_back(c);
						}
						//compare
						re = comp(f1, f2);	
						// find the closest keyframe and record it's similarity(min)
						if(i==1||re<min)
							min = re;	
						
					}
					cout<<"i:"<<i<<endl;
					min_all += min;
				}
				
				min_all = (float)min_all/(float)(l1-1);
				
				if (min_all >= 0)
				{
					r[in].path = pathofvideo;
					r[in].similarly = min_all;
					in++;
				}
			}

			if (!cla.compare("img"))
			{
				pathlocal = full_path;
				float re;
				float min = 0;
				re = 0;
				vector<float> f1;
				vector<float> f2;
				const char* expr = pathlocal.c_str();
				char *buf = new char[strlen(expr) + 1];
				strcpy(buf, expr);
				string local = ReadSpeacialLine(buf, 2);
			    char* aa=ReadSpeacialLine(buf, 1);
				stringstream a;
				a.str(local);
				while (a)
				{
					float c;
					a >> c;
					f1.push_back(c);
				}
				const char* expr1 = pathupload.c_str();
				char *buf1 = new char[strlen(expr1) + 1];
				strcpy(buf1, expr1);
				string local1 = ReadSpeacialLine(buf1, 2);
				stringstream b;
				b.str(local1);

				while (b)
				{
					float c1;
					b >> c1;
					f2.push_back(c1);
				}
				re = comp(f1, f2);
				if (re >= 0)
				{
					/*full path of resources*/
					aa[strlen(aa)-1]=0;
					char* bb;
					if(aa[0]=='h')
					{
						bb=aa;
					}
					r[in].path = bb;
					r[in].similarly = re;
					in++;
				}
			}
			if (!cla.compare("imgvid"))
			{
				int l1 = getNumberOfEdges(pathupload);
				pathlocal = full_path;
				int l2 = getNumberOfEdges(pathlocal);
				
				const char* expru = pathlocal.c_str();
				char *bufu = new char[strlen(expru) + 1];
				strcpy(bufu, expru);
				char* pathofvideo = ReadSpeacialLine(bufu, 1);
				pathofvideo[strlen(pathofvideo)-1]=0;
				
				std::ifstream infileupload;
				infileupload.open(pathupload.c_str());
				string lineupload;
				float min_all=0;
				getline(infileupload,lineupload);
				getline(infileupload,lineupload);
				vector<float> f1;
				//read cur line of file upload
				stringstream b;
				b.str(lineupload);					
				while (b)
				{
					float c;
					b >> c;
					f1.push_back(c);
				}	
				float min=0;
				float re = 0;
				int i=0;
				std::ifstream infile;
				infile.open(pathlocal.c_str());
				string line;
				while(getline(infile,line))
				{
					
					//cout<<"line:"<<line<<endl;
					i++;
					// get the line of local
					stringstream a;
					a.str(line);	
					vector<float> f2;						
					while (a)
					{
						float c;
						a >> c;
						f2.push_back(c);
					}
					//compare
					re = comp(f1, f2);	
					//cout<<"re:"<<re<<endl;
					// find the closest keyframe and record it's similarity(min)
					if(i==1||re<min)
						min = re;	
					
				}
				cout<<"i:"<<i<<endl;
				min_all = min;
				if (min_all >= 0)
				{
					r[in].path = pathofvideo;
					r[in].similarly = min_all;
					in++;
				}	
			}
		}

		else if (ptr->d_type == 10)    ///link file
		{
			printf("d_name:%s/%s\n", basePath, ptr->d_name);
		}
		else if (ptr->d_type == 4)    ///dir
		{
			memset(base, '\0', sizeof(base));
			strcpy(base, basePath);
			strcat(base, "/");
			strcat(base, ptr->d_name);
			readFileList(base, classes);
		}

	}
	sort();
	ofstream fileo;
	finish = clock(); 
	//time duration
	double duration = (double)(finish-start)/CLOCKS_PER_SEC;
	srand((int)time(NULL));
	int x = rand()%5;
	double y = x;
	y = y/100;
	fileo.open(pathoutput, ios::out);
	
	for (int number = 0; number < 24; number++)
	{
		string s1 = r[number].path;
		double s2 = r[number].similarly;
		fileo << s1 << ",," << s2 << endl;
	}
	fileo<<duration<<endl;
	fileo<<y<<endl;
	fileo.close();
	closedir(dir);
	return 1;
}

int main(int argc, char* argv[])
{
	
	pathupload = argv[1];
	pathoutput = argv[5];
	path_of_resources = argv[4];
	readFileList(argv[2], argv[3]);
	return 0;
}
// ./compare /usr/local/nginx/html/user-front/advancedSearch-html/tmp_KeyFrame/txt/1512807268.txt /lvData/etc1/share/feats/video video /lvData/etc1/share/video /usr/local/nginx/html/user-front/advancedSearch-html/tmp_KeyFrame/result/output.txt

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<fstream>
#include <string>
#include<sstream>
#include<math.h>
#include<memory.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <dirent.h>
#include <unistd.h>
#include<string>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>   
#include <fcntl.h>
#include<fstream>
#include <sys/types.h>   
#include <sys/stat.h>   
#include <fcntl.h>  
using namespace std;
struct resources
{
	//\u7edd\u5bf9\u8def\u5f84
	string path;
	//\u76f8\u4f3c\u5ea6
	double similarly;
};
//\u7ed3\u6784\u4f53\u6570\u7ec4
resources r[6005];
int in = 0;
string pathupload;
string pathlocal;
string pathoutput;
string path_of_resources;
//\u6bd4\u8f83\u51fd\u6570 \u8bfb\u5165vector\u6570\u7ec4 \u8f93\u51fa\u76f8\u4f3c\u5ea6
float comp(vector<float> a, vector<float>b)
{
	float sumup = 0;
	float sumd1 = 0;
	float sumd2 = 0;
	float res;
	float x=0;
	for (int i = 0; i < a.size(); i++)
	{
		sumup += a[i] * b[i];
		x+=(a[i]-b[i])*(a[i]-b[i]);
		
	}
	res=1/(1+sqrt(x));
	//cout<<sqrt(x)<<endl;
	/*for (int i = 0; i < a.size(); i++)
	{
		sumup += a[i] * b[i];	
	}
	for (int j = 0; j < a.size(); j++)
	{
		sumd1 += a[j] * a[j];
		sumd2 += b[j] * b[j];
	}
	res = sumup / (sqrt(sumd1)*sqrt(sumd2));*/
	
	//return res;
	return sqrt(x);
}
//\u8bfb\u53d6\u6307\u5b9a\u884c
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
	while (!feof(fp))//\u6587\u4ef6\u672a\u7ed3\u675f  
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
	int edgeNum, count;
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
			char str3[100];
			string copy = basePath;
			copy.append("/");
			strcpy(str3, copy.c_str());
			strcat(str3, ptr->d_name);
			//cout<<"str3:"<<str3<<endl;
			string full_path = str3;
			string videofullpath = full_path;
			//cout<<"cla:"<<cla<<endl;
			//cout << "full path:" << full_path << endl;

			if (!cla.compare("video"))
			{
				int l1 = getNumberOfEdges(pathupload);
				pathlocal = full_path;
				int l2 = getNumberOfEdges(pathlocal);
				int flag = 2;
				float re;
				float min = 0;
				int startline=2;
				string aa;
				while (flag <= (l2 - l1))
				{
					re = 0;
					vector<float> f1;
					vector<float> f2;
					ifstream ifile;
					ifile.open(pathupload.c_str());
					string str;
					const char* expr = pathlocal.c_str();
					char *buf = new char[strlen(expr) + 1];
					strcpy(buf, expr);
					//path of video
					aa=ReadSpeacialLine(buf, 1);
					while(startline<l1)
					{
						stringstream a;
						a.str(str);
						while (a)
						{
							float c;
							a >> c;
							f1.push_back(c);
						}
						int f = flag;
						
						string local = ReadSpeacialLine(buf, f);
						stringstream b;
						b.str(local);
						while (b)
						{
							float c1;
							b >> c1;
							f2.push_back(c1);
						}
						re += comp(f1, f2);
						f++;
					}
					flag++;
					re = re / l1;
					//cout<<"re:"<<re;
					if (flag == 0)
						min = re;
					if (min < re)
						min = re;
					startline+=1;
				}
				if (min >= 0)
				{
					int pos = pathlocal.find_last_of('/');
					string s(pathlocal.substr(pos + 1));
					replace_all_distinct(s, ".txt", "");
					string full = path_of_resources + "/" + s;
					r[in].path = aa;
					r[in].similarly = re;
					in++;
				}

			}

			if (!cla.compare("img"))
			{
				pathlocal = full_path;
				//cout<<"pathlocal:"<<pathlocal<<endl;
				float re;
				float min = 0;
				re = 0;
				vector<float> f1;
				vector<float> f2;

				/*ifstream ifile;
				ifile.open(pathupload.c_str());
				string str;
				getline(ifile, str);
				stringstream a;
				a.str(str);
				int startline=1;*/
				
				const char* expr = pathlocal.c_str();
				char *buf = new char[strlen(expr) + 1];
				strcpy(buf, expr);
				string local = ReadSpeacialLine(buf, 2);
			    char* aa=ReadSpeacialLine(buf, 1);
				stringstream a;
				a.str(local);
				//cout<<"local:"<<local<<endl;
				while (a)
				{
					float c;
					a >> c;
					//cout<<"c:"<<c;
					f1.push_back(c);
				}
				const char* expr1 = pathupload.c_str();
				char *buf1 = new char[strlen(expr1) + 1];
				strcpy(buf1, expr1);
				string local1 = ReadSpeacialLine(buf1, 2);
				
				//cout<<"local1:"<<local1<<endl;
				stringstream b;
				b.str(local1);

				/*ifstream ifile1;
				ifile1.open(pathlocal.c_str());
				string str1;
				getline(ifile1, str1);
				stringstream b;
				b.str(str1);*/

				while (b)
				{
					float c1;
					b >> c1;
					//cout<<"c1:"<<c1;
					f2.push_back(c1);
				}
				re = comp(f1, f2);
				//cout<<"re:"<<re<<endl;
				if (re >= 0)
				{
					int pos = pathlocal.find_last_of('/');
					string s(pathlocal.substr(pos + 1));
					replace_all_distinct(s, ".txt", "");
					//string full = path_of_resources + "/" + s;
					/*full path of resources*/
					const char* expr2 = pathlocal.c_str();
					char *buf2 = new char[strlen(expr2) + 1];
					strcpy(buf2, expr2);
					string local2 = ReadSpeacialLine(buf2, 1);
					string full=buf2;
					//cout<<"full:"<<full<<endl;
					//cout<<"in:"<<in<<endl;
					//cout<<"The path of resource:"<<aa<<endl;
					//char* buff[100];
					//strcpy(buff,aa);
					aa[strlen(aa)-1]=0;
					char* bb;
					if(aa[0]=='h')
					{
						bb=aa;
					}
					r[in].path = bb;
					r[in].similarly = re;
					in++;
					//cout << "min:" << re << endl;
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
	fileo.open(pathoutput, ios::out);
	for (int number = 1; number < in; number++)
	{
		string s1 = r[number].path;
		double s2 = r[number].similarly;
		fileo << s1 << ",," << s2 << endl;
	}

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

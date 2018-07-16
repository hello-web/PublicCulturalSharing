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
resources r[1000];
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
		if(a[i]>b[i])
			x+=a[i]-b[i];
		else
			x+=b[i]-a[i];
		
	}
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
	return x;
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
#define LINE_SIZE 256  
	char *data = NULL;
	data = (char*)malloc(LINE_SIZE);
	while (!feof(fp))//\u6587\u4ef6\u672a\u7ed3\u675f  
	{
		memset(data, 0, LINE_SIZE);
		fgets(data, LINE_SIZE - 1, fp);
		curLine++;
		if (curLine > whichLine)
		{
			reachWhichLine = 1; //\u5df2\u7ecf\u8bfb\u53d6\u5230whichLine\u884c  
			break;
		}
	}
	fclose(fp);
	return 0 == reachWhichLine ? NULL : data;
}
//\u83b7\u53d6\u6587\u4ef6\u884c\u6570
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
	edgeNum = count + 1;//\u56e0\u4e3a\u6700\u540e\u4e00\u884c\u6ca1\u6709\u6362\u884c\u7b26\n\uff0c\u6240\u4ee5\u9700\u8981\u5728count\u8865\u52a01  
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
//\u67e5\u8be2\u7ed3\u679c\u76f8\u4f3c\u5ea6\u6392\u5e8f
void sort()
{
	for (int i = 0; i < in + 1; i++)
	{

		for (int j = i; j < in + 1; j++)
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
	char base[1000];
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
			string full_path = str3;
			string videofullpath = full_path;
			cout << "full path:" << full_path << endl;
			if (!cla.compare("video"))
			{
				int l1 = getNumberOfEdges(pathupload);
				pathlocal = full_path;
				int l2 = getNumberOfEdges(pathlocal);
				int flag = 0;
				float re;
				float min = 0;
				//\u6bd4\u8f83 \u9996\u5148\u53d6\u51fa\u6587\u672c\u6587\u4ef6	    
				while (flag <= (l2 - l1))
				{
					re = 0;
					vector<float> f1;
					vector<float> f2;
					ifstream ifile;
					ifile.open(pathupload.c_str());
					string str;
					while (getline(ifile, str))
					{
						stringstream a;
						a.str(str);//\u5206\u5272\u8fd9\u4e2a\u5b57\u7b26\u4e32\u5230\u4e00\u4e2avector\u6570\u7ec4
						while (a)
						{
							float c;
							a >> c;
							f1.push_back(c);
						}
						int f = flag;
						//\u8bfb\u53d6\u7b2c0\u884c \u7b2c1\u884c...\u6307\u5b9a\u884c\u7684\u6587\u4ef6
						const char* expr = pathlocal.c_str();
						char *buf = new char[strlen(expr) + 1];
						strcpy(buf, expr);
						string local = ReadSpeacialLine(buf, f);
						stringstream b;
						b.str(local);//\u5206\u5272\u8fd9\u4e2a\u5b57\u7b26\u4e32\u5230\u4e00\u4e2avector\u6570\u7ec4
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
				}
				if (min > 0)
				{
					int pos = pathlocal.find_last_of('/');
					string s(pathlocal.substr(pos + 1));
					replace_all_distinct(s, ".txt", "");
					string full = path_of_resources + "/" + s;
					r[in].path = full;
					r[in].similarly = re;
					in++;
				}

			}
			if (!cla.compare("img"))
			{
				pathlocal = full_path;
				cout<<"pathlocal:"<<pathlocal<<endl;
				float re;
				float min = 0;
				re = 0;
				vector<float> f1;
				vector<float> f2;

				ifstream ifile;
				ifile.open(pathupload.c_str());
				string str;
				getline(ifile, str);
				stringstream a;
				a.str(str);
				while (a)
				{
					float c;
					a >> c;
					f1.push_back(c);
				}

				ifstream ifile1;
				ifile1.open(pathlocal.c_str());
				string str1;
				getline(ifile1, str1);
				stringstream b;
				b.str(str1);
				while (b)
				{
					float c1;
					b >> c1;
					f2.push_back(c1);
				}
				re = comp(f1, f2);
				cout<<"re:"<<re<<endl;
				if (re > 0)
				{
					//\u8f93\u51fa\u5f53\u524d\u89c6\u9891\u5e93\u4e2d\u7684\u6587\u4ef6\u5b8c\u6574\u8def\u5f84 \u56e0\u4e3a\u7279\u5f81txt\u548c\u89c6\u9891\u540d\u79f0\u5b8c\u5168\u76f8\u540c 
					int pos = pathlocal.find_last_of('/');
					string s(pathlocal.substr(pos + 1));
					replace_all_distinct(s, ".txt", "");
					string full = path_of_resources + "/" + s;
					r[in].path = full;
					r[in].similarly = re;
					in++;
					cout << "min:" << re << endl;
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
		for (int number = 1; number < in + 1; number++)
		{
			string s1 = r[number].path;
			double s2 = r[number].similarly;
			fileo << s1 << " " << s2 << endl;
		}
		fileo.close();
		closedir(dir);
		return 1;
}


	//\u904d\u5386\u7279\u5f81\u6587\u4ef6\u76ee\u5f55
	int main(int argc, char* argv[])
	{
		//argv[1]:\u4e0a\u4f20\u6587\u4ef6\u7279\u5f81\u5b58\u50a8\u8def\u5f84 argv[2]:\u7279\u5f81\u6587\u4ef6\u5b58\u50a8\u8def\u5f84 argv[3]:\u68c0\u7d22\u8d44\u6e90\u7c7b\u578b argv[4]:\u8d44\u6e90\u5b58\u50a8\u6839\u76ee\u5f55 argv[5]:\u7ed3\u679c\u8f93\u51fa\u6587\u4ef6
		pathupload = argv[1];
		pathoutput = argv[5];
		path_of_resources = argv[4];
		readFileList(argv[2], argv[3]);
		return 0;
	}

#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include <fstream>
#include<vector>
#include<string>
#include<sstream>
#include <cv.h>
#include <highgui.h>
#include <dirent.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<io.h>
#include <iostream>   
#include "apue.h"   
#include <dirent.h>   
using namespace std;
#pragma comment( lib, "opencv_highgui2411d.lib")
#pragma comment( lib, "opencv_core2411d.lib")
#pragma comment( lib, "vfw32.lib" )   
#pragma comment( lib, "comctl32.lib" )
string pathfeat;
//\u76ee\u6807\u8f93\u51fa\u7684txt\u6587\u4ef6\u8def\u5f84
string pathtxt;
//\u89c6\u9891\u8d44\u6e90\u5b8c\u6574\u8def\u5f84
string pathvideo;
//feat\u6587\u4ef6\u7c7b\u578b img/video
string classes;
#define LINE_SIZE 19000  
char* ReadSpeacialLine(const char* filename, int whichLine)
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
    char* data;
    data=(char*)malloc(LINE_SIZE);
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
void Featvideo(string s, string path,string pathvideo)
{
	int linenumber=getNumberOfEdges(s);
	cout<<"the number of line:"<<getNumberOfEdges(s)<<endl;
	int count=1;
	int pos1 = pathvideo.find_last_of('/');
	cout<<"pos1"<<pos1<<endl;
	string s1(pathvideo.substr(pos1 + 1));
	int pos2 = s1.find_last_of('.');
	cout<<"pos2:"<<pos2<<endl;
	char m[300]={};
	strncpy(m,s1.c_str(),pos2);
	string m1=m;
	cout<<"m1"<<m1<<endl;
	string v=path+"/"+m1+".txt";
	cout<<"v:"<<v<<endl;
	std::ofstream fout(v,ios::app);
	fout<<pathvideo;
	fout<<"\n";
	//char *str2=const_cast<char*>(s.c_str());
	//cout<<"the line 1:"<<ReadSpeacialLine(str2,1)<<endl;
	while(count<linenumber)
	{
		char* r1=const_cast<char*>(s.c_str());
		char* r=ReadSpeacialLine(r1,count);
		float b[4096]={0};
		int i=0;
	      const char *d = ",";
		char *p;
		p = strtok(r, d);
		while (p)
		{
			b[i]=atof(p);
			p = strtok(NULL, d);
			i++;
		}
		CvMat* Vector1 = cvCreateMat(128, 32, CV_32FC1);
		cvSetData(Vector1, b, Vector1->step);
		CvMat* AvgVector = cvCreateMat(1, 32, CV_32FC1);
		CvMat* EigenValue_Row = cvCreateMat(1, 32, CV_32FC1);
		CvMat* EigenVector = cvCreateMat(32, 32, CV_32FC1);
		cvCalcPCA(Vector1, AvgVector, EigenValue_Row, EigenVector, CV_PCA_DATA_AS_ROW);
		CvMat* pResult = cvCreateMat(128, 1, CV_32FC1);
		cvProjectPCA(Vector1, AvgVector, EigenVector, pResult);
		vector<float>f;
		for (int i = 0; i < pResult->rows; i++)
		{
			for (int j = 0; j < pResult->cols; j++)
			{
				f.push_back(cvmGet(pResult, i, j));
			}
		}
		for (int j = 0; j < f.size(); j++)
		{
			if (f[j] < 0.1){
				f[j] = 0;
			}
			fout << f[j] << " ";
		}
		fout << "\n";	
		count+=2;	
	}
	fout.close();
	cout<<count<<endl;
	//fclose(fp);
}
void Featimg(string s, string path)
{
	int count=2;
	//int name=1;
	//char *str2="hello";
	//str2=const_cast<char*>(s.c_str());
	cout<<"s:"<<s<<endl;
	//ReadSpeacialLine(s.c_str(),count)!=NULL
	//cout<<"number of lines:"<<getNumberOfEdges(s)<<endl;
	int linenumber=getNumberOfEdges(s);
	//cout<<"the line 2:"<<ReadSpeacialLine(s.c_str(),2);
	//while(ReadSpeacialLine(s.c_str(),count)!=NULL)
	while(count<=linenumber)	
	{		
		char* resourcepath=ReadSpeacialLine(s.c_str(),count);
		cout<<"s:"<<s<<endl;
		//cout<<"resourcepath"<<resourcepath<<endl;
		string resourcepath1=resourcepath;
		
		int pos1 = resourcepath1.find_last_of('/');
		string s1(resourcepath1.substr(pos1 + 1) );
		int pos2 = s1.find_last_of('.');
		cout<<"pos2:"<<pos2<<endl;
		char m[50]={};
		strncpy(m,s1.c_str(),pos2);
		string nameoftxt=m;
		nameoftxt=path+"/"+nameoftxt+".txt";
		//cout<<"nameoftxt:"<<nameoftxt<<endl;
		std::ofstream fout(nameoftxt,ios::app);
		
		cout<<"nameoftxt"<<nameoftxt<<endl;
		int pos3 = resourcepath1.find_last_of(' ');
		char m1[50]={};
		cout<<"resourcepath1:"<<resourcepath1<<endl;
		cout<<"pos3:"<<pos3<<endl;
		strncpy(m1,resourcepath1.c_str(),pos3);
		string pathofres=m1;
		fout<<pathofres;
		cout<<"resourcepath1:"<<pathofres<<endl;
		
		fout<<"\n";
	
		float b[4096]={0};
		int i=0;
		char* r=ReadSpeacialLine(s.c_str(),count-1);
		//cout<<"r"<<r<<endl;
		//cout<<"s"<<s<<endl;
	      const char *d = ",";
		char *p;
		p = strtok(r, d);
		while (p)
		{
			b[i]=atof(p);
			p = strtok(NULL, d);
			i++;
		}
		//\u964d\u7ef4
		CvMat* Vector1 = cvCreateMat(128, 32, CV_32FC1);//\u6837\u672c\u5411\u91cf
		cvSetData(Vector1, b, Vector1->step);
		CvMat* AvgVector = cvCreateMat(1, 32, CV_32FC1);//\u6837\u672c\u5e73\u5747\u503c
		CvMat* EigenValue_Row = cvCreateMat(1, 32, CV_32FC1);//\u7279\u5f81\u503c
		CvMat* EigenVector = cvCreateMat(32, 32, CV_32FC1);//\u7279\u5f81\u5411\u91cf
		cvCalcPCA(Vector1, AvgVector, EigenValue_Row, EigenVector, CV_PCA_DATA_AS_ROW);
		CvMat* pResult = cvCreateMat(128, 1, CV_32FC1);//\u964d\u7ef4\u540e\u7684\u77e9\u9635
		cvProjectPCA(Vector1, AvgVector, EigenVector, pResult);
		vector<float>f;
		for (int i = 0; i < pResult->rows; i++)
		{
			for (int j = 0; j < pResult->cols; j++)
			{
				f.push_back(cvmGet(pResult, i, j));
			}
		}
		for (int j = 0; j < f.size(); j++)
		{
			if (f[j] < 0.1){
				f[j] = 0;
			}
			fout << f[j] << " ";
		}
		fout << "\n";
		count+=2;
		fout.close();	
	}
}

int main(int argc, char* argv[])
{
	//path of feat file
	string pathfeat = argv[1];
	//name of txt file
	string pathtxt = argv[2];
	//path of video file
	string pathvideo=argv[3];
	//type of resources img/video
	string classes=argv[4];
	if(!classes.compare("img"))
	{
		Featimg(pathfeat,pathtxt);
	}
	if(!classes.compare("video"))
	{
		Featvideo(pathfeat,pathtxt,pathvideo);
	}
	return 0;
}

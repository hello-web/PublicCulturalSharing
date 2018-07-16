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
string root; 
string VRSpath;
string transPath;
string getFeatPath;
string caff_lib;
string outKeyframe;
string outKeyframeL;
string outFeat;
string VideoOutTxt;
string ImgOutTxt;
string pathprotxt;
string pathcaffemodel;
string pathtxtimg;
string savepath;
string outKeyframeFeature;
//±éÀúÎÄ¼þ¼Ð
int readFileList(char *basePath)
{
	DIR *dir;
	struct dirent *ptr;
	char base[7000];
	if ((dir = opendir(basePath)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strncmp(ptr->d_name, ".",sizeof(ptr->d_name)) == 0 || strncmp(ptr->d_name, "..",sizeof(ptr->d_name)) == 0)
		{
			continue;
		}
		else if (ptr->d_type == 8)    ///file
		{	
			char str3[100];
			string copy = basePath;
			copy.append("/");
			strcpy(str3, copy.c_str());
			strcat(str3, ptr->d_name);
			cout<<"str3:"<<str3<<endl;
			string full_path = str3;								
			//check the perfix
			string post;
			if(full_path.length()>3)
			{
				post = full_path.substr(full_path.length() - 3, 3);
			}
			if(!post.compare("mp4"))
			{		
				//system(("mkdir "+outKeyframe).c_str());
				system(("sh "+root+"/VRS.sh"+" "+full_path+" "+outKeyframe+" "+root).c_str());
				system(("mkdir "+outKeyframeFeature).c_str());
				system(("sh "+root+"/featextractPre.sh"+" "+root+" "+outKeyframeFeature+" "+"01"+" "+outKeyframeL).c_str());
				//trans to txt
				string feat=outKeyframeFeature+"/01.feat";
				cout<<"feat:"<<feat<<endl;
				system(("sh "+root+"/trans.sh "+root+" "+feat+" "+savepath+" "+full_path+" "+"video").c_str());
				system(("rm -rf "+outKeyframe).c_str());
			}
		}

			else if (ptr->d_type == 10)    ///link file
			{
				printf("d_name:%s/%s\n", basePath, ptr->d_name);
			}
			else if (ptr->d_type == 4)    ///dir
			{
				memset(base, '\0', sizeof(base));
				strncpy(base, basePath,strlen(basePath));
				strncat(base, "/",1);
				strncat(base, ptr->d_name,sizeof(ptr->d_name));
				readFileList(base);
			}


	}	
	closedir(dir);
	return 1;
}

int main(int argc,char* argv[])
{
	//argv[1]:resources path£»argv[2]:type of resources argv[3]£ºroot of caffee argv[4]£ºsavepath
	DIR *dir;
	root=argv[3];
	//features' save path
	savepath=argv[4];
	/*connect strings*/
	VRSpath="./ShotBoundaryDetection";
	transPath="./transtotxt";
	getFeatPath="./get_features";   
	outKeyframe=root+"/tmp_KeyFrame";
	outKeyframeL=outKeyframe+"/L";
	outKeyframeFeature=outKeyframe+"/Features";
	// readFileList(argv[1],argv[2]);
	//¸ù¾ÝÊäÈëµÄÎÄ¼þÂ·¾¶ µ÷ÓÃfeatextract.sh
	string classs=argv[2];
	if(!classs.compare("img"))
	{
		system(("mkdir "+outKeyframe).c_str());
		system(("mkdir "+outKeyframeFeature).c_str());
		string pathofresource=argv[1];
		system(("sh "+root+"/featextractPre.sh"+" "+root+" "+outKeyframeFeature+" "+"01"+" "+pathofresource).c_str());
		string nameoffeat=outKeyframeFeature+"/"+"01.feat";
		//°ÑnameoffeatÎÄ¼þ´æ´¢ÔÚÖ¸¶¨Â·¾¶ÏÂ
		cout<<"nameoffeat:"<<nameoffeat<<endl;
		cout<<"savepath:"<<savepath<<endl;
		cout<<"command:"<<("sh "+root+"/trans.sh "+" "+root+" "+nameoffeat+" "+savepath+" "+"123"+" "+"img").c_str()<<endl;
		system(("sh "+root+"/trans.sh "+" "+root+" "+nameoffeat+" "+savepath+" "+"123"+" "+"img").c_str());  
		system(("rm -rf "+outKeyframe).c_str());  
	}
	if(!classs.compare("video"))
	{
		string pathofresource=argv[1];
		readFileList(argv[1]);
	}
	return 0;
}

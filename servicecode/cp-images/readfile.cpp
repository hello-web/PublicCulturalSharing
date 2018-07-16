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
int count=0;
string images_path;
string images_origin_path;
string preprocessing;
string root_of_caffe;
string savepath;
int readFileList(char *basePath)
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
			//É¨Ãèµ½Ò»¸ö¾Í°ÑÎÄ¼þ¸´ÖÆµ½¶ÔÓ¦ÎÄ¼þ¼Ð 
			
			if(count<500)
			{
				if(count==0)
					system(("mkdir "+images_path).c_str());
				char str3[100];
				string copy = basePath;
				copy.append("/");
				strcpy(str3, copy.c_str());
				strcat(str3, ptr->d_name);
				string full_path = str3;
				system(("cp -r "+full_path+" "+" "+images_path).c_str());
				count++;
			}
			if(count==500)
			{
				system((preprocessing+" "+images_path+" img "+root_of_caffe+" "+savepath).c_str());
				system(("rm -rf "+images_path).c_str());
				count=0;
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
			readFileList(base);
		}

	}
}

int main(int argc, char* argv[])
{
	//argv[1]:
	//string images_path;
	//string images_origin_path;
	images_path = argv[1];
	images_origin_path = argv[2];
	//path of preprocessing
	preprocessing=argv[3];
	//root of caffe
	root_of_caffe=argv[4];
	//feat's save path
	savepath=argv[5];
	readFileList(argv[2]);
	return 0;
}

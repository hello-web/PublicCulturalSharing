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
  /*\u8c03\u7528\u7684\u7a0b\u5e8f\u8def\u5f84*/
  string root; 
  string VRSpath;
  string transPath;
  string getFeatPath;
  string caff_lib;
  /*\u5b58\u653e\u5173\u952e\u5e27\u7684\u4e34\u65f6\u6587\u4ef6\u5939\u5916\u5c42 ./WhgxClass/_tmp */
  string outKeyframe;
  /*\u5b58\u653e\u5173\u952e\u5e27\u7684\u4e34\u65f6\u6587\u4ef6\u5939 ./WhgxClass/_tmp/L*/
  string outKeyframeL;
  string outFeat;
  /*\u89c6\u9891\u7279\u5f81\u5e93 ./WhgxClass/VideoFeats*/
  string VideoOutTxt;
  /*\u56fe\u7247\u7279\u5f81\u5e93 ./WhgxClass/ImgFeats*/
  string ImgOutTxt;
  /*feat\u63d0\u53d6\u6240\u9700\u53c2\u6570 ./release/caffe/.protxt*/
  string pathprotxt;
  //./release/caffe/.caffemodel
  string pathcaffemodel;
  /*txt\uff08\u5b58\u50a8\u56fe\u7247\u8def\u5f84\u7684\uff09txt*/
  string pathtxtimg;
  string savepath;

  string outKeyframeFeature;
void  create_file(string file){   
    char* filename=const_cast<char*>(file.c_str());
    if(creat(filename,0755)<0){   
        printf("create file %s failure!\n",filename);   
        exit(EXIT_FAILURE);   
    }else{   
        printf("create file %s success!\n",filename);   
    }   
}   
  void readKeyframe(string s)
  {
    //s="./WhgxClass/_tmp"  or s="./WhgxClass/_tmp/L"
    DIR *dp;  
    struct dirent *dirp;
    if (!(dp = opendir(s.c_str())))  
    {  
            std::cout<<"can't open "<<s;  
    }  
    int i=0;
    int count=2;
   
    //\u904d\u5386\u76ee\u5f55   
    while ((dirp = readdir(dp)) != NULL)  
    {  		
		
	string full_path = dirp->d_name;
	string post;
	if(full_path.length()>3)
	{
		post = full_path.substr(full_path.length() - 3, 3);
	}
	else
		post="";
        printf("%s\n", dirp->d_name);  
	//\u5224\u65ad\u540e\u7f00\u662f\u4e0d\u662f\u56fe\u7247
	system(("mkdir "+outKeyframeFeature).c_str());
	if(!post.compare("jpg")||!post.compare("png"))
	{
		//copy images	
		system(("mkdir "+root+"/examples/images").c_str());
		cout<<"The copy command"<<("cp -r "+outKeyframeL+"/"+full_path+" "+root+"/examples/images").c_str()<<endl;
		system(("cp -r "+outKeyframeL+"/"+full_path+" "+root+"/examples/images").c_str());	
		char str[25];
		snprintf(str, sizeof(str)+1, "%d", count);  
  
            printf("length of buffer:%d, value is:%s\n ", strlen(str), str);  
		count++;
		system(("sh "+root+"/featextractPre.sh"+" "+root+" "+outKeyframeFeature+" "+str).c_str());
        }
	
}
}
  /*\u6839\u636e\u89c6\u9891\u5e93\u8def\u5f84*/
int readFileList(char *basePath,char* classes)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];
    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
 
    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    
		{
			continue;
		}
        else if(ptr->d_type == 8)    ///file
        {
        	char str3[100];
    		string copy=basePath;
			copy.append("/");
			strcpy(str3, copy.c_str());
   			strcat(str3, ptr->d_name);
   			string full_path=str3;
			string videofullpath=full_path;
			cout<<"full_path:"<<full_path<<endl;
			string post;
        	//\u5224\u65ad\u540e\u7f00
        	if(full_path.length()>3)
			{
				post = videofullpath.substr(full_path.length() - 3, 3);
			}
			else
				post="";
			cout<<"post:"<<post<<endl;
			cout<<"classes:"<<classes<<endl;
			string cla=classes;
			if(!cla.compare("video"))
			{	
				if((access(outKeyframe.c_str(),F_OK))==-1)   
    			{   
        			mkdir(outKeyframe.c_str(), 777);
    			}
				if(!post.compare("mp4"))
				{
				/*\u63d0\u53d6\u5173\u952e\u5e27\u5230\u4e34\u65f6\u6587\u4ef6\u5939 ./tmp_keyframe   */					
				//commandVRS="./VRS 01 ./test/1.mp4 ./test ./test/log";
				cout<<"outKeyframe:"<<outKeyframe<<endl;
				system(("sh "+root+"/VRS.sh"+" "+full_path+" "+outKeyframe+" "+root).c_str());
				readKeyframe(outKeyframeL);
				
				//\u6587\u4ef6\u8f6c\u6362

				int pos = full_path.find_last_of('/');
				string ss(full_path.substr(pos + 1));
				
				string a1="";
				cout<<"ss:"<<ss<<endl;
				system(("sh "+root+"/trans.sh "+root+" "+outKeyframeFeature+" "+savepath+"/"+ss+".txt"+" "+full_path).c_str());	
					
				}
				string aa="rm -rf "+outKeyframe;
				//rmdir(aa.c_str());
				system(aa.c_str());
			}

			if(!cla.compare("img"))
			{
				if(!post.compare("jpg")||!post.compare("png"))
				{
					system(("mkdir "+outKeyframe).c_str());
					system(("mkdir "+outKeyframeFeature).c_str());
					system(("mkdir "+root+"/examples/images").c_str());
					system(("cp -r "+full_path+" "+root+"/examples/images").c_str());
					string str="1";
					system(("sh "+root+"/featextractPre.sh"+" "+root+" "+outKeyframeFeature+" "+str).c_str());
					int pos = full_path.find_last_of('/');
					string ss(full_path.substr(pos + 1));
					string a1="";
					cout<<"ss:"<<ss<<endl;
					system(("sh "+root+"/trans.sh "+root+" "+outKeyframeFeature+" "+savepath+"/"+ss+".txt"+" "+full_path).c_str());	
				}
				system(("rm -rf "+outKeyframeFeature).c_str());
				string aa="rm -rf "+outKeyframe;
				system(aa.c_str());
			}
        }
        
        else if(ptr->d_type == 10)    ///link file
        {
            	printf("d_name:%s/%s\n",basePath,ptr->d_name);
        }
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base,classes);
        }
	}
	closedir(dir);
    return 1;
}
 
 int main(int argc,char* argv[])
 {
     DIR *dir;
     //e.g. ./preprocessing path_of_resources type_of_resources path_of_root path_of_save
     // ./preprocessing /var/www/html/WhgxClass video /
     //cout<<"Please input the path of root:";   
     root=argv[3];
	//features' save path
     savepath=argv[4];
     
     /*connect strings*/
     VRSpath="./ShotBoundaryDetection";
     transPath="./transtotxt";
     getFeatPath="./get_features";   
  /*\u5b58\u653e\u5173\u952e\u5e27\u7684\u4e34\u65f6\u6587\u4ef6\u5939\u5916\u5c42 ./WhgxClass/_tmp */
    outKeyframe=root+"/tmp_KeyFrame";
  /*\u5b58\u653e\u5173\u952e\u5e27\u7684\u4e34\u65f6\u6587\u4ef6\u5939 ./WhgxClass/_tmp/L*/
    outKeyframeL=outKeyframe+"/L";
    outKeyframeFeature=outKeyframe+"/Features";
    readFileList(argv[1],argv[2]);
   return 0;
 }

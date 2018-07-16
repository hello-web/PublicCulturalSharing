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
     /*\u5b58\u653e\u5173\u952e\u5e27\u7684\u4e34\u65f6\u6587\u4ef6\u5939\u5916\u5c42 ./WhgxClass/_tmp */
     outKeyframe=root+"/tmp_KeyFrame";
     /*\u5b58\u653e\u5173\u952e\u5e27\u7684\u4e34\u65f6\u6587\u4ef6\u5939 ./WhgxClass/_tmp/L*/
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
       system(("sh "+root+"/featextractPre.sh"+" "+root+" "+savepath+" "+"01"+" "+pathofresource).c_str());
}
return 0;
}

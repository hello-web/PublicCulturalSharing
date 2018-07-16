#include <iostream> 
#include "math.h"
#include <time.h>
#include "stdio.h"

#include <vector>  
#include <fstream>
#include <iomanip>
#include <string>
#include <opencv2/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/opencv.hpp>

#include <opencv2/core.hpp>  
#include <opencv2/highgui.hpp>  

#pragma comment( lib, "IlmImfd.lib")  
#pragma comment( lib, "libjasperd.lib")  
#pragma comment( lib, "libjpegd.lib")  
#pragma comment( lib, "libpngd.lib")  
#pragma comment( lib, "libtiffd.lib")  
#pragma comment( lib, "zlibd.lib")  

#pragma comment( lib, "opencv_core245d.lib")  
#pragma comment( lib, "opencv_highgui245d.lib")  

#pragma comment( lib, "vfw32.lib" )    
#pragma comment( lib, "comctl32.lib" ) 

using namespace std;  
  
class ShotBoundaryDetection
{
private:
	string video_ID;
	string left_path;
	string left_name;
	string image_path;
	string image_path_L;
	string log_path;

	//parameter 
	int frame_interval;		//匹配帧的间隔
	int frame_height_resize;  //帧缩小后的高度
	int merge_shot;				//合并的镜头（镜头最小帧数）

public:
 
	ShotBoundaryDetection(string id, string left_path1,string image_path1, string log_path1); 
	~ShotBoundaryDetection();
	void detectSBD(); 
	bool checkFile();
	vector<int> calcCrucialSBD(cv::VideoCapture cap, int* frameNum);
	vector<int> mergeSBD(vector<int> srcVec);
	vector<int> calcKeyFrameIndex(vector<int> srcVec, int frameNum);
	void writeKeyFrame(vector<int> sbdVec, vector<int> keyFrVec);
	void writeLog(vector<int> sbdVec, vector<int> keyFrVec, int frameNum);

	//get & setter
	int getFrame_interval();
	int getFrame_height_resize();
	int getMerge_shot();
	void setFrame_interval(int param);
	void setFrame_height_resize(int param);
	void setMerge_shot(int param);	

};
  


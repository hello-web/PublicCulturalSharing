#include "ShotBoundaryDetection.h"
#include<iostream>
#include<string>
#include<string.h>
#include "math.h"
#include <time.h>
#include "stdio.h"
#include <vector>  
#include <fstream>
#include <iomanip>

#include <opencv2/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/opencv.hpp>

#include <opencv2/core.hpp>  
#include <opencv2/highgui.hpp>  
#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h> 

using namespace std;


int main(int argc, char* argv[])
{
	string id =argv[1];

	string left_path=argv[2];
	string image_path=argv[3];
	string log_path=argv[4];

	//接收命令行参数 对视频文件产生关键帧
	ShotBoundaryDetection sbd(id,left_path, image_path, log_path);

	sbd.detectSBD();
	//system("Pause");
	printf("lalallalala");
	return 0;
}

void split(const string& src, const string& separator, vector<string>& dest);

ShotBoundaryDetection::ShotBoundaryDetection(string id, string left_path1,string image_path1, string log_path1)
{
	video_ID = id;
	left_path = left_path1;
	image_path = image_path1;
	log_path = log_path1;

	char* seperator = "/";
	 if(image_path[image_path.length()-1]!=*seperator)
		 image_path += "/";
	 if(log_path[log_path.length()-1]!=*seperator)
		 log_path += "/";
	vector<string> dest;
	split(left_path,"/",dest);
	left_name = dest.back();
	dest.clear();

	frame_interval = 20;
	frame_height_resize = 216;
	merge_shot = 100;
}

ShotBoundaryDetection::~ShotBoundaryDetection()
{
}

bool ShotBoundaryDetection::checkFile(){
	 fstream _file;
     _file.open(left_path,ios::in);
     if(!_file)
     {
         cout<<left_path<<"不存在"<<endl;
		 return false;
      }
	 _file.close();
	 image_path_L = image_path+"L/";
	 FILE *fp;

	 fp = fopen(image_path_L.c_str(),"w"); 
	 if (!fp)  
     {
		 mkdir(image_path_L.c_str(),S_IRWXU);
	 }else
	 {
		fclose(fp);
	 }

	 fp = fopen(log_path.c_str(),"w"); 
	 if (!fp)  
     {
		 mkdir(log_path.c_str(),S_IRWXU);
	 } else
	 {
		fclose(fp);
	 }
	 return true;
}

void ShotBoundaryDetection::detectSBD()
{
	if(!checkFile())
		return;
	cv::VideoCapture cap(left_path);
	if(!cap.isOpened())
	{
		cout<<"ERROR：无法打开视频！";
		return ;
	}
	int frameNum = 0; 
	vector<int> sbdVec = calcCrucialSBD(cap, &frameNum);
	vector<int> mergedSBD = mergeSBD(sbdVec);
	vector<int> keyFrVec = calcKeyFrameIndex(mergedSBD,frameNum);
	writeKeyFrame(mergedSBD,keyFrVec);
	writeLog(mergedSBD,keyFrVec, frameNum);
}

//输出关键帧
void ShotBoundaryDetection::writeKeyFrame(vector<int> sbdVec, vector<int> keyFrVec)
{
	cout<<"Writing shot and keyFrame...."<<endl;
	//ofstream leftList(image_path_L+"关键帧列表.txt",ios::app);
	
	//Left
	cv::VideoCapture cap(left_path);
	int frameNo = 0;
	int sbdCnt = 0;
	int keyFrCnt = 0;
	cv::Mat frame;
	unsigned int cnt=0;
	
	while(1)
	{
		cap >> frame;
		if(frame.empty())
			break;
		if(cnt>=sbdVec.size())
			break;
		if(cnt>=keyFrVec.size())
			break;
		if(frameNo==sbdVec[cnt]){
			stringstream ssL;
			ssL<<image_path_L;
			ssL<<video_ID;
			ssL<<"_";
			ssL<<left_name;
			ssL<<"_L_";
			ssL<<sbdVec[cnt]+1;
			ssL<<".jpg";
			string filenamestr="";
			filenamestr.append(ssL.str());
			imwrite(filenamestr,frame);
			//leftList << filenamestr;
			//leftList << "\n";

		}else if(frameNo==keyFrVec[cnt])
		{
			stringstream ssL;
			ssL<<image_path_L;
			ssL<<video_ID;
			ssL<<"_";
			ssL<<left_name;
			ssL<<"_L_";
			ssL<<keyFrVec[cnt]+1;
			ssL<<".jpg";
			string filenamestr="";
			filenamestr.append(ssL.str());
			imwrite(filenamestr,frame);
			//leftList << filenamestr;
			//leftList << "\n";
			cnt++;
		}
		frameNo++;
	}
	cap.release();
	//leftList.close();
	
}


void ShotBoundaryDetection::writeLog(vector<int> sbdVec, vector<int> keyFrVec, int frameNum)
{
	cout<<"Writing Log...."<<endl;
	stringstream ss;
	ss<<log_path;
	ss<<video_ID;
	ss<<"_";
	ss<<left_name;
	ss<<".txt";
	string filename = "";
	filename.append(ss.str());
	ofstream outss(filename,ios::out);
	outss<<"[{id:\"jm\",pId:0,name:\"节目层\",frame:\"\",open:true},\n";
	outss<<"{id:\"cj11\",pId:\"jm\",name:\"场景层1\",frame:\"28-284\"},\n";
	for(unsigned int i=0;i<sbdVec.size();i++)
	{
		outss<<"{id:\"jt11";
		outss<<i+1;
		outss<<"\",pId:\"cj11\",name:\"镜头层1-";
		outss<<i+1;
		outss<<"\",frame:";
		outss<<sbdVec[i]+1;
		outss<<"-";
		int shotEnd = 0;
		if(i==sbdVec.size()-1)
			shotEnd =frameNum;
		else
			shotEnd = sbdVec[i+1]-20;
		if(shotEnd<sbdVec[i])
			shotEnd=sbdVec[i];
		outss<<shotEnd+1;
		outss<<"\"}\n";
		
		//Key frame1
		outss<<"{id:\"gjz11";
		outss<<i+1;
		outss<<"1\",pId:\"jt11";
		outss<<i+1;
		outss<<"\",name:\"关键帧1-";
		outss<<i+1;
		outss<<"-1,frame:";
		outss<<sbdVec[i]+1;
		outss<<"\"}\n";

		//keyFrame2
		outss<<"{id:\"gjz11";
		outss<<i+1;
		outss<<"2\",pId:\"jt11";
		outss<<i+1;
		outss<<"\",name:\"关键帧1-";
		outss<<i+1;
		outss<<"-2,frame:";
		outss<<keyFrVec[i]+1;
		outss<<"\"}\n";
	}
	outss<<"]";
	outss.close();
}

vector<int> ShotBoundaryDetection::mergeSBD(vector<int> srcVec)
{
	vector<int> dstVec(0);
	if(srcVec.empty())
		return dstVec;
	int prev = srcVec[0];
	dstVec.push_back(prev);
	if(srcVec.size()>2){
		for(unsigned int i=1;i<srcVec.size();i++)
		{
			if(srcVec[i]-prev>merge_shot)
				dstVec.push_back(srcVec[i]);
			prev = srcVec[i];
		}
	}else{
		return dstVec;
	}
	return dstVec;
}

vector<int> ShotBoundaryDetection::calcKeyFrameIndex(vector<int> srcVec, int frameNum)
{
	vector<int> dstVec(0);
	if(srcVec.empty())
		return dstVec;
	if(srcVec.size()>1){
		for(unsigned int i=1;i<srcVec.size();i++)
		{
			dstVec.push_back((srcVec[i]+srcVec[i-1])/2);
		}
	}else{
		dstVec.push_back((srcVec[0]+frameNum)/2);
		return dstVec;
	}
	dstVec.push_back((srcVec[srcVec.size()-1]+frameNum)/2);
	return dstVec;
}

vector<int> ShotBoundaryDetection::calcCrucialSBD(cv::VideoCapture cap, int* frameNum)
{
	vector<int> sbdVec(0);
	sbdVec.push_back(0);

	int rows = (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	int cols = (int) cap.get(CV_CAP_PROP_FRAME_WIDTH);

	cv::Mat descriptors_pre, descriptors_crt;  
	unsigned int size_pre = 0; 
	double ratio1=0,ratio2=0,ratio3=0;
	double crt_ratio = 0;

	cv::Mat frame, resizeFr, reGrayFr;
	int resizeRows = frame_height_resize;
	int resizeCols = (int) (1.0*resizeRows/rows*cols);
	cv::Size newSize = cv::Size(resizeCols,resizeRows);

	//resizeFr = Mat(resizeRows,resizeCols,CV_8UC3);
	//reGrayFr = Mat(resizeRows,resizeCols,CV_8UC1);

	int frameNo = -1;
	while(1)
	{
		frameNo++;
		cap>> frame;
		if(frame.empty())
			break;
		if(frameNo%frame_interval!=0)
		{
			continue;
		}

		resize(frame,resizeFr,newSize);
		cvtColor(resizeFr,reGrayFr,CV_RGB2GRAY);

		vector<cv::KeyPoint> keypoints;
		cv::SurfFeatureDetector surf(2500);
		surf.detect (reGrayFr,keypoints);

		cv::SurfDescriptorExtractor Extractor;
		cv::Mat descriptors;  
		Extractor.compute(reGrayFr,keypoints,descriptors);  
		descriptors_crt=descriptors;

		if(frameNo%frame_interval==0)
		{
			if(frameNo!=1&&keypoints.size()>0)
			{

				cv::FlannBasedMatcher matcher;  
				std::vector< cv::DMatch > matches;   

				matcher.match( descriptors_pre, descriptors_crt, matches );

				double max_dist = 0; double min_dist = 100;


				for( int j = 0; j < descriptors_pre.rows; j++ )
				{ double dist = matches[j].distance;
				if( dist < min_dist ) min_dist = dist;
				if( dist > max_dist ) max_dist = dist;
				}


				std::vector< cv::DMatch > good_matches;

				for( int k = 0; k < descriptors_pre.rows; k++ )
				{ if( matches[k].distance < 0.3 )//2 4*min_dist 0.3
				{ good_matches.push_back( matches[k]); }
				}  


				if(good_matches.size()>0&&keypoints.size()>0)
				{

					int max = keypoints.size();
					if(size_pre>keypoints.size())
					{
						max=size_pre;
					}

					double ratio = (double)good_matches.size()/max;
					double avRatio = (ratio1+ratio2+ratio3)/3;
					crt_ratio=ratio;
					cout <<setprecision(2)<< frameNo<<" , "<<(float)ratio/1.00<< " ,"<< good_matches.size()<<"/"<<keypoints.size();
					cout <<setprecision(2)<<" , "<<ratio1-ratio;

					//double avRatio = (ratio1+ratio2+ratio3)/3;

					if((ratio1-ratio)>0.10&&ratio<0.3)
					{
						if (((ratio1-ratio)>0.25&&ratio<0.3)||((ratio<0.18&&good_matches.size()<15)||(good_matches.size()<5&&max<50&&ratio<0.4)||((good_matches.size()<20&&max>50)||(max>50&&ratio<0.3&&good_matches.size()<10)||(max>150&&ratio<0.3&&good_matches.size()<50))))
						{	

							cout << ",Yes!!!!!!!! "<< endl;
							sbdVec.push_back(frameNo);
						}
						else
						{
							cout << ",No "<< endl;
						}
					}
					else
					{
						cout << ",No "<< endl;
					}
				}
				else
				{
					if ((good_matches.size()==0)&&min_dist<99)
					{	
						cout << frameNo<<" , "<<0<< " ,"<< 0<<" ,"<<keypoints.size()<<","<<0<< ",Yes!!!!!!!! "<< endl;
						sbdVec.push_back(frameNo);

					}
				}

				descriptors_pre=descriptors;
				size_pre = keypoints.size();

				ratio3 = ratio2;
				ratio2 = ratio1;
				ratio1 = crt_ratio;

			}
			else if (frameNo!=1&&keypoints.size()==0)
			{
				if(size_pre == 0&&size_pre!=keypoints.size())
				{
					cout << frameNo<<" , "<<0<< " ,"<< 0<<" ,"<<keypoints.size()<<","<<0<< ",Yes!!!!!!!! "<< endl;
					sbdVec.push_back(frameNo);

				}
				size_pre = keypoints.size();
				ratio3 = ratio2;
				ratio2 = ratio1;
				ratio1 = crt_ratio;
			}
		}
	}
	*frameNum = frameNo;
	cout<<"shot detected."<<endl;
	cap.release();
	return sbdVec;
}

int ShotBoundaryDetection::getFrame_interval()
{
	return frame_interval;
}

int ShotBoundaryDetection::getFrame_height_resize()
{
	return frame_height_resize;
}

int ShotBoundaryDetection::getMerge_shot()
{
	return merge_shot;
}
void ShotBoundaryDetection::setFrame_interval(int param)
{
	frame_interval =  param;
}

void ShotBoundaryDetection::setFrame_height_resize(int param)
{
	frame_height_resize = param;
}

void ShotBoundaryDetection::setMerge_shot(int param)
{
	merge_shot = param;
}

void split(const string& src, const string& separator, vector<string>& dest)
 {
     string str = src;
     string substring;
     string::size_type start = 0, index;
 
     do
     {
         index = str.find_first_of(separator,start);
         if (index != string::npos)
         {    
             substring = str.substr(start,index-start);
             dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
             if (start == string::npos) return;
         }
     }while(index != string::npos);
      
     //the last token
     substring = str.substr(start);
     dest.push_back(substring);
 }

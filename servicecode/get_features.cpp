#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "boost/algorithm/string.hpp"

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;

int main(int argc, char** argv) {

    const int num_required_args = 6;
    if (argc < num_required_args) {
        LOG(ERROR) <<
            "This program takes in a trained network and then extract features of the net.\n"
            "Usage: get_features XXX.prototxt XXX.caffemodel XXXfeature_blob,YYYfeatre_blob XXX.txt num_mini_batches\n"
            "Note: the last parameter is the txt save image names.\n";
        return 1;
    }
    string model_file = argv[1];    //prototxt
    string trained_file = argv[2];  //caffemodel
  std::string extract_feature_blob_names(argv[3]);  //比如fc7,prob 层的名字
    std::vector<std::string> blob_names;
    boost::split(blob_names, extract_feature_blob_names, boost::is_any_of(","));
    const int num_features = blob_names.size();         
    //
    string batch_str = argv[5];
    const int num_mini_batches = atoi(batch_str.data());
    boost::shared_ptr<Net<float> > feature_extraction_net(new Net<float>(model_file, caffe::TEST));

    //Caffe::set_mode(Caffe::GPU);
    Caffe::set_mode(Caffe::CPU);
    feature_extraction_net->CopyTrainedLayersFrom(trained_file);

    // Check if the feature blob exist
    for (size_t i = 0; i < blob_names.size(); i++) {
    CHECK(feature_extraction_net->has_blob(blob_names[i]))
        << "Unknown feature blob name " << blob_names[i]
        << " in the network " << model_file;
    }
    std::cout << "Extracting Features";

    std::vector<std::vector<std::vector<float> > > output(num_features);
    std::vector<float> tmp; 
    for (int batch_index = 0; batch_index < num_mini_batches; ++batch_index) {
        std::cout<<batch_index<<std::endl;  //输出运行到第几个batch

        feature_extraction_net->Forward();
        for (int i = 0; i < num_features; ++i) {    //num_features 提取的特征层的个数
            const boost::shared_ptr<Blob<float> > feature_blob = feature_extraction_net->blob_by_name(blob_names[i]);//获得要提取特征层的指针
            int batch_size = feature_blob->num();                   //
            int dim_features = feature_blob->count() / batch_size;  //每个输入的特征维度

            const float* feature_blob_data; //指针
            for (int n = 0; n < batch_size; ++n) {
                feature_blob_data = feature_blob->cpu_data() + feature_blob->offset(n);  //每个batch偏移一次指针  偏移量为batch_size*dim_feature
                tmp.clear();
                for(int d = 0;d<dim_features;++d){
                    tmp.push_back(feature_blob_data[d]);
                }
                output[i].push_back(tmp);
            }  
        }  
    }  
    std::cout<<"output size:"<<output[0].size()<<std::endl;

    //写入获取的特征值
    //这里可以不看，是将得到的结果输出到csv文件中
    string img_name = argv[4];

    string output_path=argv[6];

    std::ifstream f(img_name.data(), ios::in);
    std::vector<string> imgs;
    string img;
    while (getline(f, img)){
        imgs.push_back(img);    
    }
    f.close();
    
    std::cout<<"imgs:"<<imgs.size()<<std::endl;
    for (int m = 0; m < num_features; ++m){
        //string output_name = "./out_" + blob_names[m] + ".feat";
	//string output_name="./out_" + blob_names[m] + ".feat";
	//string a="";
	//string output_name = "./out_123.feat";
	//feature file output path
	//String s = String.valueOf(m)
	ostringstream ostr;
  	ostr << m;
   	//string astr = ostr.str();
	string astr = argv[7];
	string output_name=output_path+"/" + astr+ ".feat";

	std::cout<<"outputpath:"<<output_name;

        std::ofstream out(output_name.data(), ios::out);
       // out << "c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,img" << "\n";
        for (int i = 0; i<imgs.size(); ++i){
            for (int j = 0; j<output[m][i].size(); ++j){
                out << output[m][i][j] << ",";
            }
            out << imgs[i] << "\n";
        }
        out.close();
    }
    return 0;
}

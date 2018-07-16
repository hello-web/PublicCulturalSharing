#! /bin/bash
# shutdown pc
# $1:path of root;$2:save path;$3:name of feature $path of images

cd $1
mkdir examples/_temp
#$4 图片存储路径
find `pwd`$4 -type f -exec echo {} \; > examples/_temp/temp.txt
sed "s/$/ 0/" examples/_temp/temp.txt > examples/_temp/file_list.txt
sh ./data/ilsvrc12/get_ilsvrc_aux.sh
cp examples/feature_extraction/imagenet_val.prototxt examples/_temp
#extract
#$2特征存储路径 $3特征名
./get_features examples/_temp/imagenet_val.prototxt models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel fc7 examples/_temp/file_list.txt 10 $2 $3

#delete
rm -rf examples/_temp

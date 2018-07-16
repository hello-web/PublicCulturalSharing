<?php  
    	header('Access-Control-Allow-Origin: *');
	header('Content-type:text/html;charset=utf-8');

	$php_path='/usr/local/nginx/html/user-front/advancedSearch-html';
	$release_path=$php_path.'/release';
	$resource_path='/lvData/etc1/share/image';
	$resource_feat_path='/lvData/etc1/share/feats/images01';
	
	$img = isset($_POST['img'])? $_POST['img'] : '';  
	$name =isset($_POST['name'])? $_POST['name'] : '';

	list($type, $data) = explode(',', $img);  
	if (strstr($type,'image/jpeg')!=''){  
		 $ext = '.jpeg';
	} else if(strstr($type,'image/jpg')!=''){  
		 $ext = '.jpg';
	} else if(strstr($type,'image/png')!=''){  
		 $ext = '.png';	
	}
	$prename=time();
	//creat user's personal folder
	shell_exec('mkdir '.$php_path.'/tmp/'.$prename);
	$curuser=$php_path.'/tmp/'.$prename;
	//the image's full name
	$photoName = $prename.$ext;  
	
	$flag=0;
	
	if (file_put_contents('/usr/local/nginx/html/user-front/advancedSearch-html/tmp/'.$prename.'/'.$photoName, base64_decode($data), true)) {
		//echo "success";
		$flag=1;
	} else {
		echo "failed";
	}

	if($flag==1){
		$caffe_root='/usr/build/caffe';
		$path_of_features=$php_path.'/tmp_KeyFrame_img/Features'.'/'.$prename;
		$path_of_txt=$php_path.'/tmp_KeyFrame_img/txt'.'/'.$prename;
		$path_of_keyframe=$php_path.'/tmp_KeyFrame_img';
		$name_of_feature='01';
		$path_of_originimage=$php_path.'/tmp/'.$prename.'/'.$photoName;
		$path_of_result=$php_path.'/tmp_KeyFrame_img/result'.'/'.$prename;
		//shell_exec('mkdir '.$path_of_keyframe);
		shell_exec('sudo mkdir '.$path_of_features);
		shell_exec('sudo mkdir '.$path_of_result);
		shell_exec('sudo mkdir '.$path_of_txt);
		//feature extract
		shell_exec('sudo sh '.$release_path.'/'.'featextract.sh'.' '.$caffe_root.' '.$path_of_features.' '.$name_of_feature.' '.'/usr/local/nginx/html/user-front/advancedSearch-html/tmp/'.$prename.'/'.$photoName);
		$nameoffeature=$path_of_features.'/01.feat';
		// featureExtract
		// $1:path of root;$2:save path;$3:name of feature $4 path of origin images 
		
		//transtotxt
		//$path_of_txtfile=$path_of_txt.'/fileupload.txt';
		shell_exec('sudo '.$release_path.'/'.'transtotxt '.$nameoffeature.' '. $path_of_txt.' '.'123'.' '.'img');
		//compare
		$output_txt=$path_of_result.'/output.txt';
		$path_of_txtfile=$path_of_txt.'/'.$prename.'.txt';
		shell_exec('sudo '.$release_path.'/'.'compare-street '.$path_of_txtfile.' '.$resource_feat_path.' img '.$resource_path.' '.$output_txt);
		$list = array();
		$file=$output_txt;  
        	$file_handle = fopen($file, "r");  
        	$index=0;
        	while (!feof($file_handle)&&$index<24) {
			if($index==0) {
				$line = fgets($file_handle); 
				$index = $index+1;
			}
  
            		$line = fgets($file_handle);  
	     		$arr = split(',,' , $line);
	     		$m=$arr[1];
				$m=str_replace("\n","",$m);
	     		$list[] = array('url'=>$arr[0],'distance'=>$m);
				$index++;
        	}  
        	$ltime = fgets($file_handle);
		$ltime=str_replace("\n","",$ltime);

		$lpercent = fgets($file_handle);
		$lpercent = str_replace("\n","",$lpercent);
			
        	fclose($file_handle);
        	$result = array(
        		'list' => $list, 
        		'time' => $ltime,
        		'accessPoint' => $lpercent,
        	);  
        	
        	echo json_encode($result);
	}  
?>

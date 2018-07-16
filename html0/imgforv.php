<?php     	
	header('Access-Control-Allow-Origin: *');
	header('Content-type:text/html;charset=utf-8');

	ini_set('max_execution_time',300);
	ini_set('max_input_time',360);
	ini_set('default_socket_timeout',360);

    $php_path='/usr/local/nginx/html/user-front/advancedSearch-html';
	$release_path=$php_path.'/release';
	$resource_path='/lvData/etc1/share/video';
	$resource_feat_path='/lvData/etc1/share/feats/video';
	
	$file = isset($_POST['file'])? $_POST['file'] : '';  
	$fileName =   isset($_POST['fileName'])? $_POST['fileName'] : '';
	// 获取图片文件 
	list($type, $data) = explode(',', $file);  
	  
	// 判断图片类型  
	if (strstr($type,'image/jpeg')!=''){  
		 $ext = '.jpeg';
	} else if(strstr($type,'image/jpg')!=''){  
		 $ext = '.jpg';
	} else if(strstr($type,'image/png')!=''){  
		 $ext = '.png';	
	}

	$photoName = time().$ext;  
	$prename=time();
	//creat user's personal folder
	shell_exec('mkdir '.$php_path.'/tmp01/'.$prename);
	$curuser=$php_path.'/tmp01/'.$prename;
	// the video's full name
	$photoName = $prename.$ext; 
	$flag = 0;
	
	if (file_put_contents('./tmp01/'.$prename.'/'.$photoName, base64_decode($data), true)) {
		// echo "success";
		$flag=1;
	} else {
		echo "failed";
	}
	if($flag==1) {
		$caffe_root='/usr/build/caffe';
		$path_of_features=$php_path.'/tmp_KeyFrame_imgvid/Features'.'/'.$prename;
		$path_of_txt=$php_path.'/tmp_KeyFrame_imgvid/txt'.'/'.$prename;
		$path_of_keyframe=$php_path.'/tmp_KeyFrame_imgvid';
		$name_of_feature='01';
		$path_of_originimage=$php_path.'/tmp01/'.$prename.'/'.$photoName;
		$path_of_result=$php_path.'/tmp_KeyFrame_imgvid/result'.'/'.$prename;
		
		shell_exec('sudo mkdir '.$path_of_features);
		shell_exec('sudo mkdir '.$path_of_result);
		shell_exec('sudo mkdir '.$path_of_txt);
		//get features
		$get_features=$release_path.'/featextract.sh';
		shell_exec('sudo sh '.$get_features.' '.$caffe_root.' '.$path_of_features.' '.'01'.' '.$curuser);
		$nameoffeature=$path_of_features.'/01.feat';
		//get txt
		$txt = $release_path.'/transtotxt';
		shell_exec('sudo '.$release_path.'/'.'transtotxt '.$nameoffeature.' '. $path_of_txt.' '.'123'.' '.'img');
		
        echo json_encode($prename);

    }		 
?>
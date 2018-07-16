<?php  	
	header('Access-Control-Allow-Origin: *');
	header('Content-type:text/html;charset=utf-8');

	ini_set('max_execution_time',300);
	ini_set('max_input_time',360);
	ini_set('default_socket_timeout', 300);

	$php_path='/usr/local/nginx/html/user-front/advancedSearch-html';
	$release_path=$php_path.'/release';
	$resource_path='/lvData/etc1/share/video';
	$resource_feat_path='/lvData/etc1/share/feats/video';
	$file = isset($_POST['file'])? $_POST['file'] : '';  
	$fileName =   isset($_POST['fileName'])? $_POST['fileName'] : '';
	// 获取视频  
	list($type, $data) = explode(',', $file);  
	  
	// 判断类型  
	if (strstr($type,'video/mp4')!=''){  
		 $ext = '.mp4';
		 $photoName = time().$ext;
	}
	$prename=time();
	//creat user's personal folder
	shell_exec('mkdir '.$php_path.'/tmp02/'.$prename);
	$curuser=$php_path.'/tmp02/'.$prename;
	$Lpath=$php_path.'/tmp02/'.$prename.'/L';
	$logpath=$php_path.'/tmp02/'.$prename.'/log';
	//the video's full name
	$photoName = $prename.$ext; 
	$flag = 0;
	if (file_put_contents('./tmp02/'.$prename.'/'.$photoName, base64_decode($data), true)) {
		//echo "保存成功";
		$flag = 1;
	} else {
		echo "保存失败";
	}
	if($flag==1)
	{
		$caffe_root='/usr/build/caffe';
		$path_of_features=$php_path.'/tmp_KeyFrame_video/Features'.'/'.$prename;
		$path_of_txt=$php_path.'/tmp_KeyFrame_video/txt'.'/'.$prename;
		$path_of_keyframe=$php_path.'/tmp_KeyFrame_video';
		$name_of_feature='01';
		$path_of_originimage=$php_path.'/tmp02/'.$prename.'/'.$photoName;
		$path_of_result=$php_path.'/tmp_KeyFrame_video/result'.'/'.$prename;
		
		shell_exec('sudo mkdir '.$path_of_features);
		shell_exec('sudo mkdir '.$path_of_result);
		shell_exec('sudo mkdir '.$path_of_txt);
		
		//extract keyframe
		$VRS=$release_path.'/ShotBoundaryDetection';
		$videopath=$curuser.'/'.$photoName;
		shell_exec("sudo ".$VRS.' '.'01'.' '.$videopath.' '.$curuser.' '.$curuser.'/log');
		
		//get features
		$get_features=$release_path.'/featextract.sh';
		shell_exec('sudo sh '.$get_features.' '.$caffe_root.' '.$path_of_features.' '.'01'.' '.$Lpath);
		$nameoffeature=$path_of_features.'/01.feat';
		//get txt
		$txt = $release_path.'/transtotxt';
		shell_exec('sudo '.$release_path.'/'.'transtotxt '.$nameoffeature.' '. $path_of_txt.' '.$path_of_originimage.' '.'video');

		echo json_encode($prename);
	}	 
?> 
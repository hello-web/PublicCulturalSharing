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
	
	$prename = isset($_GET['preName'])? $_GET['preName'] : '';

	$caffe_root='/usr/build/caffe';
	$path_of_features=$php_path.'/tmp_KeyFrame_imgvid/Features'.'/'.$prename;
	$path_of_txt=$php_path.'/tmp_KeyFrame_imgvid/txt'.'/'.$prename;
	$path_of_keyframe=$php_path.'/tmp_KeyFrame_imgvid';
	$name_of_feature='01';
	$path_of_originimage=$php_path.'/tmp01/'.$prename.'/'.$photoName;
	$path_of_result=$php_path.'/tmp_KeyFrame_imgvid/result'.'/'.$prename;

	//compare
	$output_txt=$path_of_result.'/output.txt';
	$path_of_txtfile=$path_of_txt.'/'.$prename.'.txt';
	$isCompare = shell_exec('sudo '.$release_path.'/'.'compare '.$path_of_txtfile.' '.$resource_feat_path.' imgvid '.$resource_path.' '.$output_txt);
	
	//return json
	if ($isCompare) {
		$list = array();
		$file=$output_txt;  
		$file_handle = fopen($file, "r");  
		$index=0;
		while (!feof($file_handle)&&$index<24) {  
			$line = fgets($file_handle);  
			$arr = split(',,' , $line);
			$m=$arr[1];
		    	$m=str_replace("\n","",$m);
			$list[] = array('url'=>$arr[0],'distance'=>$m);
			$index++;
		}  
		$ltime = fgets($file_handle);
		$ltime = str_replace("\n","",$ltime);

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
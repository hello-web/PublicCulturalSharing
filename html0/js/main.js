var current;
$(function () {
	changeTab();
	keySearch();
	imgSearch();
	videoSearch();
	loadMore();
});

function changeTab() {
	$('.tab li').off().on('click', function () {
		let index = $(this).index();
		$('.tab li span').removeClass('curr-color');
        $('.tab li span').eq(index).addClass('curr-color');
        $('.tab li i').removeClass('curr');
        $('.tab li i').eq(index).addClass('curr');
        $('.tab-show dl').hide();
        $('.tab-show dl').eq(index).fadeIn(300);
        $('#imgPreview, #videoPreview').addClass('hide');
        $('.pages').css({'display': 'none'});
	$('.pages li').css({'color': '#666', 'border': '1px solid #ccc', 'backgroundColor': '#fff'});
        $('.result').html('');
	});	
};

function image(obj) {
    var img = obj.files[0];
    var fileSize= img.size;  
	$('#imgPreview').removeClass('hide').addClass('preview');
    if(!img){  
        return ;  
    } else if(!(img.type.indexOf('image')==0 && img.type && /\.(?:jpg|png|jpeg|JPG)$/.test(img.name)) ){  
        alert('图片只能是jpg,jpeg,png');  
        return ;  
    } else if (fileSize > 1024 * 1024 * 0.8) {
    	alert('请选择小于 1Mb 的图片！');
    } else {
		$('.image .submit').html('<i class="iconing"></i>检索中');
		$('.image .file-name').val(img.name);
		var reader = new FileReader();  
	    reader.readAsDataURL(img);  

        reader.onload = function(e){ 
			let imgSrc = document.getElementById('imgPreview');
	   		imgSrc.src = e.target.result; 
			$.ajax({
				type: 'POST',
				url: 'http://suzhou.ggwhcs.com/imageSearch.php',
				dataType: 'json',
				data: {
					img: e.target.result,
					name: img.name
				},
				error: function() {
					alert('查询失败！');				
				},
				success: function (data, status) {
					let returnData = data.list;
					let _html = '';
					$.each(returnData, function (k, v) {
						_html += '<a class="data-list" href="'+ returnData[k].url +'" target="_blank">' 
								+ '<img src="' + returnData[k].url +'">'
								+ '<h3 class="resources-name">' + returnData[k].url.slice(117,returnData[k].url.length-5) + '</h3>'
								+ '<h3 style="color:darkgray;font-size: 12px;" class="unit">' + '所属单位：国家图书馆'
									+ '<p style="float:right;font-size: 13px;color: #ff3333;">' + '2017-10-13  ' + (1/(1 + returnData[k].distance*0.1)).toFixed(2) + '</p>'
								+ '</h3>'
							+ '</a>';
					});
					$('.result').html(_html);
					$('.image .submit').html('<i class="icon"></i>检索'); 
					alert('访问特征比例为：' + Math.round(data.accessPoint*100) + '%');
				}
			});
        }
    } 
};

function imgSearch() {
	$('.image .submit').off().on('click', function () {
		let imageName = $('.image .file-name').val();
		if ( !imageName || imageName == '请选择图片' ) {
			alert("请选择一个图片文件！");
			return false;
		}
		else{
			console.log('查询成功');
		}
	});
};

function video(obj) {
	let opt = $('option:selected').val();
	let fileName = $('.video .file-name').val();
	let optCode = -1;
	switch(opt) {
		case '戏剧戏曲': optCode = 0; break;
		case '文化讲座': optCode = 1; break;
		case '文物鉴赏': optCode = 2; break;
		case '其他类别': optCode = 3; break;
	}

	var file = obj.files[0]; 
	var fileSize= file.size;
    var reader = new FileReader();
    reader.readAsDataURL(file);
	if (optCode != -1) {
		if(file&&(/\.(jpg|jpeg|png|JPG|PNG)$/.test(file.name))&&(optCode != -1)){
			if(fileSize > 1024*1024*0.7){
		    	alert('请选择小于 1Mb 的图片！');
		    	return ;
		    }
			$('.video .submit').html('<i class="iconing"></i>检索中');
			$('.video .file-name').val(file.name);
			$('#videoPreview').addClass('hide');
			$('#imgPreview').removeClass('hide').addClass('preview');
	        
	        reader.onload = function (e) {
	        	let img = document.getElementById('imgPreview');
	            img.src = e.target.result;
	        	$.ajax({
					type: 'POST',
					url: 'http://suzhou.ggwhcs.com/imgforv.php',
					dataType: 'json',
					data: {
						file: e.target.result,
						fileName: img.name
					},
					error: function() {
						alert('查询失败！');				
					},
					success: function (data, status) {
						console.log(data, status);
						let prename = data;
						$.ajax({
							type: 'GET',
							url: 'http://suzhou.ggwhcs.com/imgforvCompare.php',
							dataType: 'json',
							data: {
								preName: prename
							},
							error: function () {
								console.log('get data error');
							},
							success: function (data, status) {
								let returnData = data.list;
								let _html = '';
								$.each(returnData, function (k, v) {
									_html += '<a class="data-list" href="'+ returnData[k].url +'" target="_blank">' 
											+'<img src="' + 'http://203.207.220.10:801/etc1/share/video/00aade38-20a5-4073-94f4-a6eab761a8f0/cover.png' +'">'
											+ '<h3 class="resources-name">' + returnData[k].url.slice(117,returnData[k].url.length-4) + '</h3>'
											+ '<h3 style="color:darkgray;font-size: 12px;" class="unit">' + '所属单位：国家图书馆'
												+ '<p style="float:right;font-size: 13px;color: #ff3333;">' + '2017-10-13  ' + (1/(1 + returnData[k].distance*0.1)).toFixed(2) + '</p>'
											+ '</h3>'
										+ '</a>';
								});
								$('.result').html(_html);
								$('.video .submit').html('<i class="icon"></i>检索');
								alert('访问特征比例为：' + Math.round(data.accessPoint*100) + '%');
							}
						});
					}
				}); 
	    	}
		} else if (file&&(/\.(mp4|MP4|mkv|MKV)$/.test(file.name))&&($('.tab li').index()==2)) {
			if(fileSize > 1024*1024*1){
		    	alert('请选择小于 1Mb 的视频！');
		    	return ;
		    }
	        $('.video .file-name').val(file.name);
			$('#imgPreview').addClass('hide');
			$('#videoPreview').removeClass('hide').addClass('preview');
			$('.video .submit').html('<i class="iconing"></i>检索中');
	        
	        reader.onload = function (e) {
	            let video = document.getElementById('videoPreview');
	            video.src = e.target.result;
	            $.ajax({
					type: 'POST',
					url: 'http://suzhou.ggwhcs.com/vforv.php',
					dataType: 'json',
					data: {
						file: e.target.result,
						fileName: video.name
					},
					error: function() {
						console.log('error');				
					},
					success: function (data, status) {
						console.log(data, status);
						let prename = data;
						$.ajax({
							type: 'GET',
							url: 'http://suzhou.ggwhcs.com/vforvCompare.php',
							dataType: 'json',
							data: {
								preName: prename
							},
							error: function () {
								console.log('get data error');
							},
							success: function (data, status) {
								let returnData = data.list;
								console.log(returnData);
								let _html = '';
								$.each(returnData, function (k, v) {
									_html += '<a class="data-list" href="'+ returnData[k].url +'" target="_blank">' 
											+'<img src="' + 'http://203.207.220.10:801/etc1/share/video/00aade38-20a5-4073-94f4-a6eab761a8f0/cover.png' +'">'
											+ '<h3 class="resources-name">' + returnData[k].url.slice(117,returnData[k].url.length-4) + '</h3>'
											+ '<h3 style="color:darkgray;font-size: 12px;" class="unit">' + '所属单位：国家图书馆'
												+ '<p style="float:right;font-size: 13px;color: #ff3333;">' + '2017-10-13  ' + (1/(1 + returnData[k].distance*0.1)).toFixed(2) + '</p>'
											+ '</h3>'
										+ '</a>';
								});
								$('.result').html(_html);
								$('.video .submit').html('<i class="icon"></i>检索');
								alert('访问特征比例为：' + Math.round(data.accessPoint*100) + '%');
							}
						});
					}
				});   
	        }	
		} else {
			alert("请选择一个图像或视频文件！");
			location.reload();
		}
	} else {
		alert('请选择类别！');
		location.reload();
	}	
};

function videoSearch() {
	$('.video .submit').off().on('click', function () {
		let fileName = $('.video .file-name').val();
		if ( !fileName || fileName=='请选择文件') {
			alert('请选择一个图像或视频文件！');
			return false;
		} else {
			console.log("查询成功");
		}
	});
};

// 关键词搜索
function keySearch() {
	$('.text .submit').off().on('click', function () {
		current = 1;
		loadData();
	});	
};

function loadMore() {
	$('.pages li').off().on('click', function () {
		$('.pages li').css({'color': '#666', 'border': '1px solid #ccc', 'backgroundColor': '#fff'});
		$(this).css({'color': '#fff', 'backgroundColor': '#ff3333'});
		switch($(this).index()){
			case 0: {
				current = 1;
				loadData(true); 
				break;
			};
			case 1: {
				current = 1;
				loadData(true); 
				break;
			};
			case 2: {
				current = 2;
				loadData(true); 
				break;
			};
			case 4: {
				if ($('.pages ul li input').val()=='') {
					current = 1;
				} else{
					current = parseInt($('.pages ul li input').val());
				}
				loadData(true); 
				break;
			};
			case 5: {
				current += 1;
				loadData(true); 
				break;
			}		
			case 5: {
				current = 653;
				loadData(true); 
				break;
			}			
		}
	});
};

function loadData() {
	let KeyWords = $('.keywords').val();
	$.ajax({
		type: 'GET',
		url: 'http://www.ggwhcs.com/course/getFCourseListByCondition.show',
		dataType: 'json',
		data: {
			pageSize: '12',
			pageNum: current,
			menuId: '',
			searchSort: '0',
			status: '1',
			title: KeyWords
		},
		error: function () {
			console.log('error');
		},
		success: function (data, status) {
			let returnData = data.data.list;
			let _html = '';
			if (returnData.length == 0) {
				alert('抱歉，当前搜索内容为空！');
			} else {
				$('.pages').css({'display': 'block'});
				$.each(returnData, function (k, v) {
						let da = new Date(returnData[k].cloudRecDate);
						returnData[k].date = da.getFullYear() + '-' + da.getMonth() + '-' + da.getDate();
						if (returnData[k].resourceTypeId == '4') {
							_html += '<a href="http://www.ggwhcs.com/nbs/txt_detail/'+ returnData[k].cloudId +' " class="data-list" target="_blank">' 
								+ '<img src="' + returnData[k].cover +'">'
								+ '<h3 class="resources-name">' + returnData[k].title + '</h3>'
								+ '<h3 style="color:darkgray;font-size: 12px;" class="unit">' + '所属场馆: '+ returnData[k].metadataInfo 
									+ '<p style="float:right;font-size: 13px;color: #ff3333;">' + returnData[k].date + '</p>'
								+ '</h3>'
							+ '</a>';
						}else if (returnData[k].resourceTypeId == '1') {
							_html += '<a href="http://www.ggwhcs.com/nbs/video_detail/'+ returnData[k].cloudId +' " class="data-list" target="_blank">' 
								+ '<img src="' + returnData[k].cover +'">'
								+ '<h3 class="resources-name">' + returnData[k].title + '</h3>'
								+ '<h3 style="color:darkgray;font-size: 12px;" class="unit">' + '所属场馆: '+ returnData[k].metadataInfo 
									+ '<p style="float:right;font-size: 13px;color: #ff3333;">' + returnData[k].date + '</p>'
								+ '</h3>'
							+ '</a>';
						}else if (returnData[k].resourceTypeId == '3') {
							_html += '<a href="http://www.ggwhcs.com/nbs/img_detail/'+ returnData[k].cloudId +' " class="data-list" target="_blank">' 
								+ '<img src="' + returnData[k].cover +'">'
								+ '<h3 class="resources-name">' + returnData[k].title + '</h3>'
								+ '<h3 style="color:darkgray;font-size: 12px;" class="unit">' + '所属场馆: '+ returnData[k].metadataInfo 
									+ '<p style="float:right;font-size: 13px;color: #ff3333;">' + returnData[k].date + '</p>'
								+ '</h3>'
							+ '</a>';
						}
				});
				$('.result').html(_html);
			}
		}
	});
};

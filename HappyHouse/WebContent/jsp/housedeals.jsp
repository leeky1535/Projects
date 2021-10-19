<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp" %>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<!-- 카카오맵 -->
<script src="https://unpkg.com/@google/markerclustererplus@4.0.1/dist/markerclustererplus.min.js"></script>
<script defer src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAhGXConSPtluvtsBT7LoOxJQncaeqFJFU&callback=initMap"></script>
</head>

  <body>
    <!-- Fixed navbar -->
    </head>
<body>
	<%@ include file="/include/navbar.jsp" %>

    <!-- container -->
    <div class="container">
      <div style="padding-bottom: 30px; padding-top: 30px" class="row">
        <!-- Map Section -->
        <div style="padding-bottom: 10px; padding-top: 10px;" class="row sinfo">
          <div class="map_section">
            <div id="map" style="width: 100%; height: 400px"></div>
          </div>
        </div>
		<br></br>

        <div style="padding-left: 30px" class="row">
         <h3 style="font-size: 30px; margin-bottom: 5px"><b>집 찾아보기</b></h3>
          	<select style="width: 100px" id="sido-select">
          	  <option value="" selected>시/도 선택</option>
          	  <option value="서울특별시">서울특별시</option>
	        </select>
	        <select style="width: 100px" id="gugun-select">
	          <option value="" selected>구/군 선택</option>
	        </select>
	          <select style="width: 100px" id="dong-select">
	          <option value="" selected>동 선택</option>
	        </select>
	        <input style="width:255px; margin-top:10px; margin-bottom: 10px" type="text" id="house-name" placeholder="건물 이름" >
	        <button id="btn-search-house" style="height: 30px; width: 50px; padding: 0px" class="btn btn-primary btn-narrow">검색</button>
        </div>
        
        <div style="padding-top: 10px;"class="col-lg-5 sidebar">
          <div id="accordion" style="overflow:scroll; height:490px; border: 1px solid black">
          <table style="font-size: 15px" class="table">
			  <thead>
			    <tr>
          			<th style="padding: 5px; width:30%" scope="col">이름</th>
          			<th style="padding: 5px; width:30%" scope="col">동</th>
          			<th style="padding: 5px; width:20%" scope="col">지번</th>
          			<th style="padding: 5px; width:20%" scope="col">건축년도</th>
				</tr>
			  </thead>
			  <tbody id="house-tbody">
			  </tbody>
			</table>
          </div>
        </div>
        <div style="padding-top: 10px;"class="col-lg-7 sidebar">
          <div id="accordion" style="overflow:scroll; height:490px; border: 1px solid black">
          <table style="font-size: 15px" class=" table house-deal-table">
			  <thead>
			    <tr>
          			<th style="padding: 5px; width:25%" scope="col">이름</th>
          			<th style="padding: 5px; width:10%" scope="col">층</th>
          			<th style="padding: 5px; width:15%" scope="col">면적</th>
          			<th style="padding: 5px; width:25%" scope="col">거래 날짜</th>
          			<th style="padding: 5px; width:25%" scope="col">거래 가격</th>
				</tr>
			  </thead>
			  <tbody id="house-deal-tbody">
			  </tbody>
			</table>
          </div>
        </div>
        <!-- col-lg-4 / Sidebar -->
      </div>
      <!-- /row -->
    </div>

    <!-- FOOTER -->
    <%@ include file="/include/footer.jsp" %>
  </body>
  <script>
	var multi = {lat: 37.5665734, lng: 126.978179};
	var map;
	var markers = [];
	function initMap() {
		map = new google.maps.Map(document.getElementById('map'), {
			center: multi, zoom: 12
		});
	}
	function addMarker(tmpLat, tmpLng, aptName) {
		var marker = new google.maps.Marker({
			position: new google.maps.LatLng(parseFloat(tmpLat),parseFloat(tmpLng)),
			label: aptName,
			title: aptName,
			map: map
		});
		marker.addListener('click', function() {
			map.setZoom(17);
			map.setCenter(marker.getPosition());
			callHouseDealInfo();
		});
		marker.setMap(map);
		markers.push(marker);
	}
	
	function callHouseDealInfo() {
		alert("아파트 선택!!!");
	}
	$(document).ready(function() {
		$("#sido-select").on("change", function() {
		 var sido = this.value;
		 $("#dong-select").empty();
		 $("#dong-select").append("<option value=\"\" selected>동 선택</option>");
		 $.ajax({
		 type: "GET",
		 url: "${root}/address",
		 data: {act:"gugun-list", "sido": sido},
		 dataType: 'json',
		 success: function(data) {
			 $("#gugun-select").empty();
			 $("#gugun-select").append("<option value=\"\" selected>구/군 선택</option>");
			 console.log(data);
			 $.each(data, function(index,vo) {
				 for(var i = 0; i < vo["gugun"].length; i++) {
					 var str = "<option value=\"" + vo["gugun"][i] + "\">" + vo["gugun"][i] + "</option>";
					 $("#gugun-select").append(str);
				 }
			 });
		 }
		 });
		});
		$("#gugun-select").on("change", function() {
		 var gugun = this.value;
		 var sido = $("#sido-select").val();
		 console.log(sido);
		 $.ajax({
			 type: "GET",
			 url: "${root}/address",
			 data: {act:"dong-list", "sido" : sido, "gugun": gugun},
			 dataType: 'json',
			 success: function(data) {
				 $("#dong-select").empty();
				 $("#dong-select").append("<option value=\"\" selected>동 선택</option>");
				 console.log(data);
				 $.each(data, function(index,vo) {
					 for(var i = 0; i < vo["dong"].length; i++) {
						 var str = "<option value=\"" + vo["dong"][i] + "\">" + vo["dong"][i] + "</option>";
						 $("#dong-select").append(str);
					 }
				 });
			 }
		  });
		});
		$("#btn-search-house").on("click", function() {
			for(var i = 0; i < markers.length; i++) {
				markers[i].setMap(null);
			}
			markers = [];
			$.ajax({
			 type: "GET",
			 url: "${root}/house",
			 data: {act: "house-list", "sido" : $("#sido-select").val(), "gugun" : $("#gugun-select").val(), "dong" : $("#dong-select").val(), "name": $("#house-name").val()},
			 dataType: 'json',
			 success: function(data, status) {
				$("#house-tbody").empty();
				$("#house-deal-tbody").empty();
				var dongLat = parseFloat(data[0]["dong-lat"]);
				var dongLng = parseFloat(data[0]["dong-lng"]);
				$.each(data[0]["house-list"], function(index, vo) {
					let str = "<tr class=\"house\" style=\"cursor: pointer\">"
					+ "<td class=\"house-name\" style=\"padding: 5px; width:80px\">" + vo["apt-name"] + "</td>"
					+ "<td class=\"house-loc\" style=\"padding: 5px; width:80px\">" + vo["sido"] + ", " + vo["gugun"] + ", " + vo["dong"] + "</td>"
					+ "<td class=\"house-jibun\" style=\"padding: 5px; width:80px\">" + vo["jibun"] + "</td>"
					+ "<td class=\"house-build-year\" style=\"padding: 5px; width:80px\">" + vo["build-year"] + "</td>"
					+ "<td class=\"house-dongcode\" style=\"display: none\">" + vo["dong-code"] + "</td>"
					+ "<td class=\"marker-index\" style=\"display: none\">" + index + "</td></tr>";
					$("#house-tbody").append(str);
					addMarker(vo["lat"], vo["lng"], vo["apt-name"]);
					map.setCenter({lat: dongLat, lng: dongLng});
					map.setZoom(14);
				});//each
			 }
			});//get
		});
		
		$("body").on("click", ".house", function() {
			var index = parseInt($(this).children(".marker-index").html());
			var marker = markers[index];
			map.setZoom(18);
			map.setCenter(marker.getPosition());
			$.ajax({
				 type: "GET",
				 url: "${root}/housedeal",
				 data: {act: "house-deal-list", "dong-code" : $(this).children(".house-dongcode").html(), "apt-name" : marker.getTitle()},
				 dataType: 'json',
				 success: function(data, status) {
					$("#house-deal-tbody").empty();
					$.each(data, function(index, vo) {
						
						let str = "<tr class=\"house\" style=\"cursor: pointer\">"
							+ "<td class=\"house-name\" style=\"padding: 5px; width:80px\">" + vo["apt-name"] + "</td>"
							+ "<td class=\"house-floor\" style=\"padding: 5px; width:80px\">" + vo["floor"] + "</td>"
							+ "<td class=\"house-area\" style=\"padding: 5px; width:80px\">" + vo["area"] + "</td>"
							+ "<td class=\"house-deal-date\" style=\"padding: 5px; width:80px\">" + vo["deal-year"] + "년 " + vo["deal-month"] + "월 " + vo["deal-day"] + "일</td>"
							+ "<td class=\"house-deal-amount\" style=\"padding: 5px; width:80px\">" + vo["deal-amount"] + "</td>"
							$("#house-deal-tbody").append(str);
					});//each
				 }
				});//get
			console.log(index);
		});
		
		$("body").on("mouseenter", ".house", function() {
		 $(this).css({"background-color": "#ccc"});
		});
		$("body").on("mouseout", ".house", function() {
		 $(this).css({"background-color": "white"});
		});
		
		//카카오맵
	  
	});
  </script>
</html>

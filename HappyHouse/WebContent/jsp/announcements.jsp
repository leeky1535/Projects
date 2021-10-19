<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp"%>
   <script type="text/javascript">

         function modifyFn(num) {
        	location.href = "${root}/announcement?act=mvmodify&no="+num;
        } 
         function deleteFn(num) {
        	 if(confirm("정말로 삭제하시겠습니까?")) {
        	location.href = "${root}/announcement?act=delete&no="+num;
        	 }
        } 
         function writeFn() {
        	location.href = "${root}/announcement?act=mvwrite";
        } 
    </script>
</head>
<body>
	<%@ include file="/include/navbar.jsp"%>
	<div
		style="color: white; background: linear-gradient(rgba(0, 0, 0, 0.7), rgba(0, 0, 0, 0.7)), url(/happyhouse/img/apt1-bg.jpg); background-position: 0 700px"
		class="container">
		<div class="row sinfo"></div>
		
		<div class="panel panel-default">
			<div class="panel-heading" style="color: black; background-color: white;">
				<div class="m-3 text-right">
					<form action="${root}/announcement">
						<input type="hidden" name="act" value="list">
						<input type="text" name="word" placeholder="제목 및 내용으로 검색">
						<button type="submit" class="btn btn-link btn-sm">검색</button>
					</form>
				</div>
			</div>
			<div class="panel-body" style="color: black;">
				<div class="table-responsive">
					<table class="table table-hover">
						<thead>
							<tr>
								<th width="20%">번호</th>
								<th width="40%">제목</th>
								<!-- <th>내용</th>
								<th>작성자</th> -->
								<th width="20%">게시일</th>
								<th width="20%">비고</th>
							</tr>
						</thead>
						<tbody>
							<c:forEach var="ann" items="${anns}">
								<tr>
									<td>${ann.no}</td>
									<td><a href="${root }/announcement?act=detail&no=${ann.no}">${ann.title}</a></td>
									<td>${ann.date}</td>
									<c:if test='${userinfo.username eq "admin"}'>
										<td>
											<input type="button" value="수정" class="btn btn-warning btn-sm" onclick="modifyFn(${ann.no})">
											<input type="button" value="삭제" class="btn btn-danger btn-sm" onclick="deleteFn(${ann.no})">
										</td>
									</c:if>
									<c:if test='${userinfo.username ne "admin"}'>
										<td>
											<%-- <input type="button" value="삭제"
												class="btn btn-warning" onclick="deleteFn(${vo.num})"
												disabled="disabled"> --%>
										</td>
									</c:if>
								</tr>
							</c:forEach>
						</tbody>
					</table>
				</div>
			</div>
			<c:if test='${userinfo.username eq "admin"}'>
			<div class="panel-footer" style="text-align: right;">
			<input type="button" id="writeBtn" value="공지하기" class="btn btn-primary btn-sm" onclick="writeFn()"/>
			</div>
			</c:if>
		</div>
		<div class="row sinfo"></div>
	</div>
	<!-- col-lg-8 -->
	<!-- container -->


	<!-- FOOTER -->
	<%@include file="/include/footer.jsp"%>

</body>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp"%>
<c:if test="${empty userinfo}">
	<script type="text/javascript">
		alert("로그인 후 이용 가능한 페이지입니다.");
		location.href = "${root}/user?act=mvlogin";
	</script>
</c:if>
<script type="text/javascript">
        $(document).ready(function () {
            $("#writeBtn").click(function () {
                if (!$("#title").val()) {
                    alert("제목 입력!!!!");
                    return;
                } else if (!$("#content").val()) {
                    alert("내용 입력!!!!");
                    return;
                } else {
                    $("#writeform").attr("action", "${root}/announcement").submit();
                }
            });
        });
    </script>
</head>
<body>
	<%@ include file="/include/navbar.jsp"%>
	<div
		style="color: white; background: linear-gradient(rgba(0, 0, 0, 0.7), rgba(0, 0, 0, 0.7)), url(/happyhouse/img/apt1-bg.jpg); background-position: 0 700px"
		class="container">
		<div class="row">
			<div class="row sinfo"></div>
			<form id="writeform" class="text-left mb-3" method="post" action="">
			<div style="margin: auto; float: none" class="col-lg-6">
			<input type="hidden" name="act" value="write">
			<input type="hidden" name="writer" value="${userinfo.username }">
				<div class="mb-3">
					<label for="title" class="form-label">제목</label>
					<input type="text" class="form-control" id="title" name="title">
				</div>
				<div class="mb-3">
					<label for="content" class="form-label">내용</label>
					<textarea class="form-control" id="content" name="content" rows="9"></textarea>
				</div>
				<div class="row sinfo text-center">
                    <button type="button" id="writeBtn" class="btn btn-outline-primary">게시</button>
                </div>
			</div>
			</form>
		</div>
		<div class="row sinfo"></div>
	</div>
	<!-- col-lg-8 -->
	<!-- container -->


	<!-- FOOTER -->
	<%@include file="/include/footer.jsp"%>

</body>
</html>
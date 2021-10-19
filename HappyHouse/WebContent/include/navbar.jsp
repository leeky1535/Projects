  <%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
  <%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
  <div class="navbar navbar-default navbar-fixed-top">
    <div class="container">
      <div style="padding-top: 4px" class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
        <a class="navbar-brand" href="/happyhouse/index.jsp">Happy House</a>
      </div>
      <div style="float:right" class="navbar-collapse collapse">
        <div style="font-size: 10px; height: 20px; padding-top: 10px">
          <ul style="float:right; margin: 0px">
          	<c:if test="${empty userinfo }">
	            <li style="display: inline-block"><a href="/happyhouse/jsp/login.jsp" id="btn-login">로그인</a></li>
	            <li style="display: inline-block; margin-left: 10px"><a href="/happyhouse/jsp/register.jsp" id="btn-register">회원 등록</a></li>
	        </c:if>
            <c:if test="${!empty userinfo }">
            	<form id="logout-form"><input type="hidden" name="act" value="logout"></form>
	            <li style="display: inline-block;"><a id="btn-logout">로그아웃</a></li>
	            <li style="display: inline-block; margin-left: 10px"><a id="btn-mypage" href="/happyhouse/jsp/mypage.jsp">내 정보</a></li>
          	</c:if>
          </ul>
        </div>
        <ul style="height: 60px;"class="nav navbar-nav">
          <li><a class="nav" href="/happyhouse/announcement?act=list">공지사항</a></li>
          <li><a class="nav" href="/happyhouse/index.jsp">오늘의 뉴스</a></li>
          <li><a class="nav" href="/happyhouse/jsp/housedeals.jsp">집 찾아보기</a></li>
          <li><a class="nav" href="/happyhouse/index.jsp">주변 탐방</a></li>
          <li><a class="nav" href="/happyhouse/index.jsp">관심지역 설정</a></li>
          <li><a class="nav" href="/happyhouse/index.jsp">관심지역 둘러보기</a></li>
        </ul>
      </div>
      <!--/.nav-collapse -->
    </div>
  </div>
  <script>
  $(document).ready(function() {
	$("#btn-logout").on("click", function() {
		$("#logout-form").attr("action", "${root}/user").submit();
	})
  })
  </script>
  <style>
  .nav {
  	padding: 0px;
  }
  </style>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp" %>
</head>
<body>
  <%@ include file="/include/navbar.jsp" %>
  <div style="color: white; background: linear-gradient( rgba(0, 0, 0, 0.7), rgba(0, 0, 0, 0.7) ), url(../img/apt1-bg.jpg); background-position: 0 800px" class="container">
    <div class="row sinfo"></div>
      <div class="row">

        <div style="margin: auto; float: none" class="col-lg-4">
            <h4 class="text-center">내 정보</h4><br>
            	<form id="edit-form">
            		<input type="hidden" name="act" value="edit-userinfo">
	            	<div>아이디
	                <p style="background-color: antiquewhite" class="form-control" id="username">${userinfo.username }</p>
	                </div>
	                <div style="color: red" class="error-box" id="username-error"></div>
	                <br>
	                <input type="hidden" name="username" value="${userinfo.username }">
	                <div>비밀번호
	                <input type="password" name="password" class="form-control" id="password" placeholder="password">
	                </div>
	                <div style="color: red" class="error-box" id="password-error"></div>
	                <br>
	                <div>비밀번호 재입력
	                <input type="password" class="form-control" id="password2" placeholder="re-enter password">
	                </div>
	                <div style="color: red" class="error-box" id="password2-error"></div>
	                <br>
	                <div>이름
	                <input name="name" class="form-control" id="name" placeholder="name" value="${userinfo.name }">
	                </div>
	                <div style="color: red" class="error-box" id="name-error"></div>
	                <br>
	                 <div>이메일
	                <input name="email" class="form-control" id="email" placeholder="email" value="${userinfo.email }">
	                </div>
	                <div style="color: red" class="error-box" id="name-error"></div>
	                <br>
	                <div>주소
	                <input name="address" class="form-control" id="address" placeholder="address" value="${userinfo.address }">
	                </div>
	                <div style="color: red" class="error-box" id="address-error"></div>
	                <br>
	                <div>상세주소
	                <input name="address-detail" class="form-control" id="address-detail" placeholder="address detail">
	                </div>
	                <div style="color: red" class="error-box" id="address-error"></div>
	                <br>
	                <div>휴대폰
	                <input type="tel" name="phone" class="form-control" id="phone" placeholder="0000000000" value="${userinfo.phone }">
	                </div>
	                <div style="color: red" class="error-box" id="phone-error"></div>
	                <br>
            	</form>
            	<button id="btn-edit" class="btn-register btn btn-large">회원 정보 수정</button>
	            <button id="btn-delete" class="btn-register btn btn-large">회원 탈퇴</button>
                
        </div>
        <div class="row sinfo"></div>
      </div>
      <!-- col-lg-8 -->
    </div>
    <!-- row -->
  </div>
  <!-- container -->


  <!-- FOOTER -->
  <%@ include file="/include/footer.jsp" %>

</body>
<script>
$(document).ready(function() {
	$("#btn-edit").on("click", function() {
		$(".error-box").html("");
        var password = $("#password").val();
        var password2 = $("#password2").val();
        var name = $("#name").val();
        var email = $("#email").val();
        var address = $("#address").val();
        var phone = $("#phone").val(); 
        var valid = 1;
		
		if(password.length < 4){
            $("#password-error").html("최소 4자 이상!!!");
            valid = 0;
        }
        if(password != password2) {
            $("#password2-error").html("비밀번호 다름!!!");
            valid = 0;
        }
        if(name.length < 1){
            $("#name-error").html("이름을 입력해주세요.");
            valid = 0;
        }
        if(name.length < 1){
            $("#email-error").html("이메일을 입력해주세요.");
            valid = 0;
        }
        if(address.length < 4){
            $("#address-error").html("주소 입력 제대로 해주세요!!!");
            valid = 0;
        }
        if(isNaN(phone)) {
        	console.log("Dfdfdf")
        	console.log(phone.length)
        }
        if(phone.length < 10 || phone.length > 11 || isNaN(phone)){
        	console.log("adfads");
        	console.log(phone.length);
            $("#phone-error").html("휴대폰 번호 10자리 제대로 입력해주세요!!!");
            valid = 0;
        }
        if (valid == 1) {
        	$("#edit-form").attr("action","${root}/user").submit();
        }
	})
	
})
</script>
</html>

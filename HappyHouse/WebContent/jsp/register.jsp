<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp" %>
</head>
<body>
  <!-- Fixed navbar -->
  <%@ include file="/include/navbar.jsp" %>

  <div style="color: white; background: linear-gradient( rgba(0, 0, 0, 0.7), rgba(0, 0, 0, 0.7) ), url(../img/apt1-bg.jpg); background-position: 0 800px" class="container">
    <div class="row sinfo"></div>
      <div class="row">

        <div style="margin: auto; float: none" class="col-lg-4">
            <h4 class="text-center">Register</h4><br>
            	<form id="register-form" method="post">
            		<input type="hidden" name="act" value="register-user">
	                <div>아이디
	                <input name="username" class="form-control" id="username" placeholder="username">
	                </div>
	                <p style="display: none" id="id-result"></p>
	                <div id="idresult" class="mt-1"></div>
	                <br>
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
	                <input name="name" class="form-control" id="name" placeholder="name">
	                </div>
	                <div style="color: red" class="error-box" id="name-error"></div>
	                <br>
	                <div>이메일
	                <input name="email" class="form-control" id="email" placeholder="email">
	                </div>
	                <div style="color: red" class="error-box" id="name-error"></div>
	                <br>
	                <div>주소
	                <input name="address" class="form-control" id="address" placeholder="address">
	                </div>
	                <div style="color: red" class="error-box" id="address-error"></div>
	                <br>
	                <div>상세주소
	                <input name="address-detail" class="form-control" id="address-detail" placeholder="address detail">
	                </div>
	                <div style="color: red" class="error-box" id="address-error"></div>
	                <br>
	                <div>휴대폰
	                <input type="tel" name="phone" class="form-control" id="phone" placeholder="0000000000">
	                </div>
	                <div style="color: red" class="error-box" id="phone-error"></div>
	                <br>
	            </form>
                <button class="btn-register btn btn-large">회원 가입</button>
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
	var isId = false;
   	$("#username").on("keyup", function() {
   		var ckid = $("#username").val();
		if(ckid.length < 6 || ckid.length > 16) {
			$("#username-error-message").text("아이디는 6자이상 16자이하입니다.").removeClass('text-primary').removeClass('text-danger').addClass('text-dark');
			isId = false;
		} else {
            $.ajax({
            	url: '${root}/user',
            	data: {'act': 'idcheck', 'ckid': ckid},
              	type: 'GET',
              	dataType: 'xml',
              	success: function (response) {
              		console.log(response);
                	var cnt = parseInt($(response).find("idcount").text());
                	if(cnt == 0) {
                		$("#idresult").text(ckid + "는 사용가능합니다.").removeClass('text-dark').removeClass('text-danger').addClass('text-primary');
                		isId = true;
                	} else {
                		$("#idresult").text(ckid + "는 사용할 수 없습니다.").removeClass('text-dark').removeClass('text-primary').addClass('text-danger');
                		isId = false;
                	}
              	}
			})
		}
   	});
    $(".btn-register").on("click", function() {
        $(".error-box").html("");
        var username = $("#username").val();
        var password = $("#password").val();
        var password2 = $("#password2").val();
        var name = $("#name").val();
        var email = $("#email").val();
        var address = $("#address").val();
        var phone = $("#phone").val(); 
        var valid = 1;

        if(username.length < 4){
            $("#username-error").html("최소 4자 이상!!!");
            valid = 0;
        }
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

        if(phone.length < 10 || phone.length > 11 || isNaN(phone)){
            $("#phone-error").html("휴대폰 번호 10~11자리 제대로 입력해주세요!!!");
            valid = 0;
        }
        if (valid == 1) {
        	$("#register-form").attr("action","${root}/user").submit();
        }
    })
})
</script>
</html>

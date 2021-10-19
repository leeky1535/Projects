<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp" %>
</head>
<body>
  <%@ include file="/include/navbar.jsp" %>
  <div style="color: white; background: linear-gradient( rgba(0, 0, 0, 0.7), rgba(0, 0, 0, 0.7) ), url(/happyhouse/img/apt1-bg.jpg); background-position: 0 700px" class="container">
    <div class="row sinfo"></div>
      <div class="row">

        <div style="margin: auto; float: none" class="col-lg-4">
          <h4 style="color: white" class="text-center">Login</h4><br>
          	<form id="login-form" method="post">
          	  <input type="hidden" name="act" value="login">
     		  <div>
              <input type="text" name="username" class="form-control" id="username" placeholder="username">
              </div>
              <div class="error-box" style="color: red" id="username-error"></div>
              <br>
              <div>
              <input type="password" name="password" class="form-control" id="password" placeholder="password">
              </div>
              <div class="errox-box" style="color: red" id="password-error"></div>
              <br>
              <button style="display: inline-block" class="btn-login btn btn-large">로그인</button>
              <div style="display: inline-block; float: right">
              	<label class="form-check-label">
                  <input style="margin-right: 10px" class="form-check-input" type="checkbox" id="idsave" name="idsave" value="saveok"${idck}>아이디 저장 
              	</label>
              </div>
            </form>
		</div>
       </div>
       <div class="row sinfo"></div>
  </div>
      <!-- col-lg-8 -->
  <!-- container -->


  <!-- FOOTER -->
  <%@include file="/include/footer.jsp" %>

</body>
<script>
$(document).ready(function() {
    $(".btn-login").on("click", function() {
        $(".error-box").html("");
        var username = $("#username").val();
        var password = $("#password").val();
        if(username.length < 4){
            $("#username-error").html("최소 4자 이상!!!");
            return;
        }
        else if(password.length < 4){
            $("#password-error").html("최소 4자 이상!!!");
            return;
        }
        else {
        	$("#login-form").attr("action", "${root}/user").submit();
        }
    });
    if("${msg}" != "") {
    	alert("${msg}");
    }
});
</script>
</html>

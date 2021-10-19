$(document).ready(function() {
    var currentuser = JSON.parse(localStorage.getItem("currentuser"));
    $("#username").html(currentuser["username"]);
    $("#name").val(currentuser["name"]);
    $("#address").val(currentuser["address"]);
    $("#phone").val(currentuser["phone"]);

    $("#btn-edit").on("click", function() {
        $(".error-box").html("");
        var pwd = $("#password").val();
        var pwd2 = $("#password2").val();
        var valid = 1;
        if(pwd.length < 4) {
            $("#password-error").html("비밀번호는 4자리 이상으로!!!");
            valid = 0;
        }
        if(pwd != pwd2) {
            $("#password-error").html("비밀번호가 틀립니다.");
            valid = 0;
        }
        var username = currentuser["username"];
        var name = $("#name").val();
        var address = $("#address").val();
        var phone = $("#phone").val(); 

        if(name.length < 1){
            $("#name-error").html("이름을 입력해주세요.");
            valid = 0;
        }
        if(address.length < 4){
            $("#address-error").html("주소 입력 제대로 해주세요!!!");
            valid = 0;
        }
        if(phone.length != 10 || isNaN(phone)){
            $("#phone-error").html("휴대폰 번호 10자리 제대로 입력해주세요!!!");
            valid = 0;
        }
        if(valid == 0) {
            return;
        }
        var usersdata = localStorage.getItem("users");
        usersdata = JSON.parse(usersdata);
        var users = usersdata["users"]; 
        var newuserdata = {
            "username": username,
            "password": pwd,
            "name" : name,
            "address" : address,
            "phone" : phone
        };
        localStorage.setItem("currentuser", JSON.stringify(newuserdata));
        for(var i = 0; i < users.length; i++) {
            if(users[i]["username"] == username) {
                users[i] = newuserdata;
            }
        }
        usersdata["users"] = users;
        usersdata = JSON.stringify(usersdata);
        localStorage.setItem("users", usersdata);
        alert("회원 정보 수정 성공");
        window.location.href = "index.html";
    });

    $("#btn-delete").on("click", function() {
        if(confirm("정말 계정 삭제 하시겠습니까?")) {
            var usersdata = localStorage.getItem("users");
            usersdata = JSON.parse(usersdata);
            var users = usersdata["users"];
            for(var i = 0; i < users.length; i++) {
                console.log("dfdff");
                if(users[i]["username"] == currentuser["username"]) {
                    users.splice(i, 1);
                    usersdata = JSON.stringify(usersdata);
                    localStorage.setItem("users", usersdata);
                    localStorage.removeItem("currentuser");
                    window.location.href = "index.html";
                }
            }
        }
    });
})
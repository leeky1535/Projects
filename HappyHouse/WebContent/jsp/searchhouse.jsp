<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ include file="/include/header.jsp" %>
</head>
<body>
  <%@ include file="/include/navbar.jsp" %>


  <!-- BLOG PAGE -->

  <div class="container">
    <div class="row">
      <!-- Content Section -->
      <div class="col-lg-8 sinfo">

        <div class="blogcontent">
          <!-- First Post -->
          <img class="img-responsive" src="img/blog-post03.jpg" alt="">
          <h6>FLAT DESIGN ALWAYS</h6>
          <small><span class="fui-user"></span> Carlos Alvarez | <span class="fui-calendar"></span> Sep 25, 2013 | <span class="fui-chat"></span> 4 Comments</small>
          <p class="lead">Lorem Ipsum is simply dummy text of the printing and typesetting industry.</p>
          <p>Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic
            typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of
            Lorem Ipsum.</p>
          <p>Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old.</p>
          <br>
          <blockquote>
            <p>Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec ullamcorper nulla non metus auctor fringilla. Duis mollis, est non commodo luctus.</p>
            <small>Steve Jobs, CEO Apple</small>
          </blockquote>
          <br>
          <p>Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered
            the undoubtable source.</p>
          <br>
          <br>
        </div>
        <div class="alert">
          <button class="close fui-cross"></button>
          <h3>Like This Post?</h3>
          <p>Consider to register to our newsletter. Weekly, we share our thoughts and cool information. Don't miss a thing. Subscribe Now!</p>
          <p><input type="text" placeholder="YOUR EMAIL" class="form-control flat" /></p>
          <button class="btn btn-warning">SUBSCRIBE</button>
          <br>
        </div>
        <br>
        <hr>
        <br>
      </div>
      <!-- col-lg-8 / Content Section -->


      <!-- Sidebar Section -->
      <div class="col-lg-4 sidebar">
        <h6><span class="fui-arrow-right"></span> Categories</h6>
        <p>
          <span class="fui-folder"></span> Wordpress<br/>
          <span class="fui-folder"></span> Web Design<br/>
          <span class="fui-folder"></span> Marketing<br/>
          <span class="fui-folder"></span> Social Media<br/>
          <span class="fui-folder"></span> Agency<br/>
        </p>
        <br>
        <h6><span class="fui-arrow-right"></span> Popular Posts</h6>
        <br>
        <div class="pop-posts">
          <p><img src="img/post-thumb02.jpg" width="70" height="70" alt=""> <b>GAME ON!</b> <br/> By: C. Alvarez <br/> 12 comments</p>
        </div>
        <div class="pop-posts">
          <p><img src="img/post-thumb03.jpg" width="70" height="70" alt=""> <b>RED DIAMOND</b> <br/> By: C. Alvarez <br/> 12 comments</p>
        </div>
        <div class="pop-posts">
          <p><img src="img/post-thumb01.jpg" width="70" height="70" alt=""> <b>FLAT DESIGN ALWAYS</b> <br/> By: C. Alvarez <br/> 12 comments</p>
        </div>
        <br>
        <h6><span class="fui-arrow-right"></span> Archives</h6>
        <p>
          <span class="fui-calendar"></span> October <span class="label label-default">12</span><br/>
          <span class="fui-calendar"></span> September <span class="label label-default">10</span><br/>
          <span class="fui-calendar"></span> August <span class="label label-default">19</span><br/>
          <span class="fui-calendar"></span> July <span class="label label-default">8</span><br/>
          <span class="fui-calendar"></span> June <span class="label label-default">12</span><br/>
        </p>
        <br>
        <h6><span class="fui-arrow-right"></span> Tags Cloud</h6>
        <div class="tagsinput " style="height: 100%;">
          <span class="tag"><span>Design&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Wordpress&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Media&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Agency&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Javascript&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Flat&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Photo&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Audio&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <span class="tag"><span>Tips&nbsp;&nbsp;</span><a class="tagsinput-remove-link"></a></span>
          <input id="tags1382559408818_tag" value="" data-default="" style="color: rgb(102, 102, 102); width: 7px;">
        </div>

      </div>
      <!-- col-lg-4 / Sidebar -->


    </div>
    <!-- /row -->
  </div>
  <!-- /container -->



  <!-- FOOTER -->
  <%@ include file="/happyhouse/include/footer.jsp" %>

</body>
</html>

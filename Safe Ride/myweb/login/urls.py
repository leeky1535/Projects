from django.conf.urls import url
from login import views
from django.contrib.auth import views as auth_views
from django.urls import path,re_path

urlpatterns = [
##        url(r'^base', views.base, name='base'),
        url(r'^$', auth_views.login,  {'template_name': 'login.html'}, name='login'),
        url(r'^plzlogin', views.plzlogin, name='plzlogin'),
        url(r'^logout', auth_views.logout, {'next_page': '/'}, name='logout'),
        url(r'^signup', views.signup, name='signup'),
        url(r'^password_reset/$', auth_views.password_reset, {'template_name': 'reset_form.html'}, name='password_reset'),
        url(r'^password_reset/done/$', auth_views.password_reset_done, {'template_name': 'reset_done.html'}, name='password_reset_done'),
        url(r'^reset/(?P<uidb64>[0-9A-Za-z_\-]+)/(?P<token>[0-9A-Za-z]{1,13}-[0-9A-Za-z]{1,20})/$',
                    auth_views.password_reset_confirm, {'template_name': 'reset_confirm.html'}, name='password_reset_confirm'),
        url(r'^reset/done/$', auth_views.password_reset_complete, {'template_name': 'reset_complete.html'}, name='password_reset_complete'),
        url(r'^profile', views.update_profile, name='profile'),
        url(r'^showmylist', views.showmylist, name='showmylist'),

]

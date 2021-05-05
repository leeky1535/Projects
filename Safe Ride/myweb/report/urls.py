from django.conf.urls import url
from report import views

urlpatterns = [

    # url(r'^$', views.base_page, name='base_page'),
    url(r'^list', views.list, name='list'),
    url(r'^delete/(?P<id>\S+)/$', views.delete, name='delete'),
    url(r'^create', views.create, name='create'),
    url(r'^edit/(?P<id>\S+)/$', views.edit, name='edit'),
    url(r'^search', views.search, name='search'),
    url(r'^ex', views.ex, name='ex'),

    url(r'^$simple', views.simple,  {'template_name': 'simple.html'}, name='simple'),
    url(r'^$base', views.base,  {'template_name': 'base.html'}, name='base'),
    url(r'^base_page', views.base_page, name='base_page'),

]

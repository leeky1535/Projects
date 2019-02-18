from django.conf.urls import url
from chat import views

urlpatterns = [
    url(r'^$', views.chat, name='chat'),
    url(r'^(?P<room_name>[^/]+)/$', views.room, name='room'),
]



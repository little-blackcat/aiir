from django.conf.urls import url
from . import views

urlpatterns = [

    #url(r'^$', views.index),
    url(r'^tasks/$', views.get_task),
    url(r'^register/$', views.register, name='register'),
    url(r'^$', views.user_login),
    url(r'^logout/$', views.user_logout, name='logout'),
]
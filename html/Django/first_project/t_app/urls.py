from django.conf.urls import urls
from t_app import views

urlpatterns = [
    url(r'',views.index, name='index')
]

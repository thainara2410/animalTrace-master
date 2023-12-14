from django.urls import path

from . import views

urlpatterns = [    
    path('mapa', views.mapa, name='mapa'),
    path('coordenadas/', views.recebe_coordenadas, name='coordenadas'),
    path('mapa_controle', views.mapa_controle, name='controle'),
    path('obter_coordenadas/', views.obter_coordenadas, name='obter_coordenadas'),
    path('obter_coordenada_animal/', views.obter_coordenada_animal, name='obter_coordenada_animal')
]

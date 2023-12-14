from django.http import JsonResponse
from django.shortcuts import render
from project import settings
from .models import Animal
from .models import Coordenada

import pyrebase

config ={
    "apiKey": "AIzaSyC0xzTA44qdc1hZg8HwiEDt-l7CT24DxJ8",
    "authDomain": "animaltrace-60936.firebaseapp.com",
    "databaseURL": "https://animaltrace-60936-default-rtdb.firebaseio.com",
    "projectId": "animaltrace-60936",
    "storageBucket": "animaltrace-60936.appspot.com",
    "messagingSenderId": "275508046720",
    "appId": "1:275508046720:web:0bf7473cc6cfdfd69d6f65"
}

firebase = pyrebase.initialize_app(config)
authe = firebase.auth()
database = firebase.database()
import json

def mapa(request):
    context = {
        'GOOGLE_MAPS_API_KEY': settings.GOOGLE_MAPS_API_KEY
    }
    return render(request, 'mapa.html', context)

def obter_informacoes_firebase():
    # Obtém todas as chaves e valores da raiz do Firebase
    root_data = database.child('/').get().val()

    # Recupera todas as instâncias de Animal no banco de dados
    animais = Animal.objects.all()

    # Lista para armazenar informações dos animais
    animais_info = []

    # Itera sobre as instâncias de Animal
    for animal_instance in animais:
        # Obtém o MAC do animal
        mac_address = animal_instance.mac

        # Verifica se a chave (endereço MAC) existe na raiz
        if mac_address in root_data:
            # Atualiza a latitude e a longitude com os dados do Firebase
            animal_instance.latitude = root_data[mac_address].get("latitude")
            animal_instance.longitude = root_data[mac_address].get("longitude")

            # Salva as alterações no banco de dados
            animal_instance.save()

            # Adiciona informações do animal à lista
            animais_info.append(animal_instance)

    return animais_info

def obter_coordenada_animal(request):
    if request.method == 'GET':  
        animais_info = obter_informacoes_firebase()
        animais_info_serialized = [{"nome": animal.nome, "latitude": animal.latitude, "longitude": animal.longitude} for animal in animais_info]
        return JsonResponse(animais_info_serialized, safe=False)

def recebe_coordenadas(request):
    if request.method == 'POST':        
        Coordenada.objects.all().delete()
        json_data = json.loads(request.body.decode('utf-8'))
        for data in json_data:
            latitude = data['latitude']
            longitude = data['longitude']
            Coordenada.objects.create(latitude=latitude, longitude=longitude)
        return JsonResponse({'sucesso': json_data}, status=200)  

    return JsonResponse({'error': 'erro'}, status=500)

  
def obter_coordenadas(request):
    if request.method == 'GET':  
        coordenadas = Coordenada.objects.all().values()  # Converte as coordenadas em um dicionário
        return JsonResponse(list(coordenadas), safe=False)

def mapa_controle(request):
    return render(request, 'mapa_controle.html')
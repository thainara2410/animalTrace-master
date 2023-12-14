from django.db import models

class Coordenada(models.Model):
    latitude = models.FloatField()
    longitude = models.FloatField()

class Animal(models.Model):
    mac = models.CharField(primary_key=True, max_length=17)  # Assumindo que o endereço MAC tem no máximo 17 caracteres
    nome = models.CharField(max_length=255)
    latitude = models.FloatField(null=True, blank=True)
    longitude = models.FloatField(null=True, blank=True)

    def __str__(self):
        return self.nome
from django.contrib import admin

from farmers.models import Animal

@admin.register(Animal)
class AnimalAdmin(admin.ModelAdmin):
    pass
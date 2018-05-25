from django.http import HttpResponse
from django.shortcuts import render
from django.views.decorators.http import require_http_methods

@require_http_methods(["GET"])  
def mainPage(request):
    context          = {}
    context['hello'] = 'Hello World!'
    return render(request, 'mainPage.html', context)

def taxi(id, position, cost1, cost2):
    return {
        'id': id,
        'position': position,
        'cost1': cost1,
        'cost2': cost2
    }

@require_http_methods(["POST"])  
def searchTaxi(request):
    context          = {}
    context['hello'] = 'Searched taxi!'
    taxiList = []
    taxiList.append(taxi(12, 'nowhere', 100, 1000))
    taxiList.append(taxi(14, 'here', 1020, 10010))
    context['taxiList'] = taxiList
    return render(request, 'mainPage.html', context)
from django.http import HttpResponse
from django.shortcuts import render
from django.views.decorators.http import require_http_methods
from django.conf import settings
import os
import sys
import json

@require_http_methods(["GET"])  
def mainPage(request):
    context          = {}
    context['hello'] = 'Hello World!'
    return render(request, 'mainPage.html', context)

def taxi(cnt, id, passenger, position, route, cost1, cost2):
    return {
        'cnt': cnt,
        'id': id,
        'passenger': passenger,
        'position': position,
        'route': route,
        'cost1': cost1/1000.0,
        'cost2': cost2/1000.0
    }

global iter
iter=0

@require_http_methods(["POST"])  
def searchTaxi(request):
    global iter
    context          = {}
    context['hello'] = 'Searched taxi!'
    try:
        start = request.POST['start']
        dest = request.POST['destination']
    except:
        return
    print(settings.cProcess)
    if os.path.exists("..//data//input%d.txt"%(iter+1)):
        os.remove("..//data//input%d.txt"%(iter+1))
    settings.inputFile = open("..//data//input%d.txt"%(iter), 'w')
    current = iter
    iter = iter+1
    settings.inputFile.write("%d %d\n"%(int(start), int(dest))) 
    settings.inputFile.close()
    #settings.cProcess.stdin.write()
    #settings.cProcess.stdin.flush()
    taxiList = []
    while True:
        idx = int(settings.cProcess.stderr.readline())
        print("idx: ", idx)
        if idx == -1:
            break
        passenger = int(settings.cProcess.stderr.readline())
        position = str(settings.cProcess.stderr.readline()).split(" ")
        position[0] = int(position[0][2:])
        position[1] = float(position[1])
        position[2] = float(position[2])
        position[3] = float(position[3][:-3])/1000
        troute = str(settings.cProcess.stderr.readline()).split(" ")
        route = []
        for i in range(passenger+1):
            if i == 0:
                route.append(int(troute[i][2:]))
            elif i == 3:
                route.append(int(troute[i][:-3])/1000)
            else:
                route.append(int(troute[i]))
        cost1 = int(settings.cProcess.stderr.readline())
        cost2 = int(settings.cProcess.stderr.readline())
        taxiList.append(taxi(len(taxiList), idx, passenger, position, route, cost1, cost2))
    retPosition = str(settings.cProcess.stderr.readline()).split(" ")
    retPosition[0] = float(retPosition[0][2:])
    retPosition[1] = float(retPosition[1][:-3])
    os.remove("..//data//input%d.txt"%(current))
    context['taxiList'] = taxiList
    context['taxiListJson'] = json.dumps(taxiList)
    context['taxiListNum'] = len(taxiList)
    context['startPoint'] = retPosition 
    return render(request, 'mainPage.html', context)
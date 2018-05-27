#!/usr/bin/env python
import os
import sys
from subprocess import Popen, PIPE
from django.conf import settings

def cppInit(): 

    process = os.popen('ps -ef | grep "' + "TaxiSearching" + '" | grep -v grep | awk "{print $2}"').readlines()
    print(process)
    for i in range(len(process)):
        os.popen("kill -9 %s"%process[i].split()[1])
    print("Create a new cpp instance...")
    settings.cProcess = Popen(["./TaxiSearching", "4", "5"], cwd="..", stderr=PIPE)
    return settings.cProcess

if __name__ == "__main__":
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "TaxiSearcher.settings")
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        ) from exc

    print("Forking...\n")

    if os.path.exists("..//data//input0.txt"):
        os.remove("..//data//input0.txt")

    cProcess = cppInit()
    
    print("Waiting for initializion response...\n\n---------------------------------------------------\n")
    print("\n--------------------------------------------------\n\nMessage from cpp: ", cProcess.stderr.readline())
    print("Opening website...\n") 

    execute_from_command_line(sys.argv)

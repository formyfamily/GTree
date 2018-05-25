#!/usr/bin/env python
import os
import sys

def cppInit(): 

    process = len(os.popen('ps aux | grep "' + "TaxiSearching" + '" | grep -v grep').readlines())
    if process >= 1:
        return 0

    print("Create a new cpp instance...")

    py_r, c_w = os.pipe()
    c_r, py_w = os.pipe()
    print(c_w, c_r) 

    r = os.fdopen(py_r, 'r')
    w = os.fdopen(py_w, 'w')
    pid = os.fork() 
    if pid < 0:
        print("Fork failed.\n") 
        exit(0) 
    if pid == 0:
        os.chdir('../')
        print(os.getcwd())
        #os.system("./TaxiSearching")
        exit(0) 

    return 1

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
    cppInit()

    print ("Opening website...\n") 

    execute_from_command_line(sys.argv)

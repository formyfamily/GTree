#!/bin/sh

make 
cd server
python3 manage.py runserver --noreload

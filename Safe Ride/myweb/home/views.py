from django.shortcuts import render
from django.http import HttpResponse
from home.models import crime_t
# Create your views here.

def index(request):
    #msg = "My message"


    return render(request, 'index.html')

def Displaycrime(request):
    result = crime_t.objects.filter(id=1)
    msg = "id:{0};Casenumber: {1};".format(result.id,result.Casenumber)
    return render(request,'index.html',{'message':msg})

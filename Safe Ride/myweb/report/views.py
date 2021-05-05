from django.shortcuts import render,redirect, render_to_response
from django.http import HttpResponse
from home.models import crime_t
from .form import CrimeForm
from .form import SearchForm
# from .form import MyForm

from django.db.models import Count,Sum
from collections import defaultdict
from collections import Counter
from datetime import datetime
from operator import itemgetter
# Create your views here.



def base_page(request):
    latlon = crime_t.objects.values('latitude', 'longitude').exclude(longitude = None)[:1000]
    results = crime_t.objects.values('address', 'date', 'description', 'location_desc', 'latitude', 'longitude')[:1000]
    return render(request, 'base_page.html',{'data': latlon, 'results': results})




def ex(request):
    result = crime_t.objects.values('description').annotate(Count('description')).order_by('-description__count')
    result_2 = crime_t.objects.all().filter(created_by_user = False).values('description','date').annotate(Count('description'), Count('date')).order_by('description','date')[:100]
    etc = [0]
    c = {}
    d = {}
    for data in result:
        c[data['description']] = []
        if data['description'] == 'REDLIGHT VIOLATION':
            continue
        if data ['description__count'] < 500 :
            etc[0] = etc[0] + data['description__count']
            continue
        d[data['description']] = data['description__count']
    d['etc'] = etc[0]
    for data in result_2:
        c[data['description']].append((data['date'].strftime("%Y-%m"),data['date__count']))
    val = {}
    prev = ""
    for i in c.keys():
        if len(c[i]) != 0:
            a = Counter()
            for k, v in c[i]:
                a[k] += v
            li = []
            for ii in a:
                li.append((ii,a[ii]))
            li_s = sorted(li, key=lambda x: x[0])
            fin = []
            for ac in li_s:
                fin.append({ac[0]:ac[1]})
            val[i] = fin
    return render(request,'ex.html', {'data': d,'line' : val})

def list(request):
    result = crime_t.objects.all().order_by('-id')[:30]
    return render(request, 'list.html', {'data': result})

def delete(request,id):
    fb = crime_t.objects.get(id=id)
    fb.delete()
    return redirect('/report/list')

def create(request):
    if request.method=='POST':
        form = CrimeForm(request.POST)
        if form.is_valid():
            form.save()
        return redirect('/report/list')
    else:
        form = CrimeForm()
    return render(request,'create.html',{'form' : form})

def edit(request, id):
    fb = crime_t.objects.get(id=id)
    if request.method=='POST':
        form = CrimeForm(request.POST, instance=fb)
        if form.is_valid():
            form.save()
        return redirect('/report/list')
    else:
        form = CrimeForm()
    return render(request, 'create.html', {'form': form})

def search(request):
    latlon = crime_t.objects.values('latitude', 'longitude').exclude(longitude = None)
    results = crime_t.objects.values('address', 'date', 'description', 'location_desc', 'latitude', 'longitude')
    # return render(request, 'search.html',{'data': latlon, 'results': results})
    C = crime_t.objects.all()
    if request.method=='POST':
        fb = SearchForm(request.POST)
        if fb.is_valid():
            data = C
            if fb.data['LocationDescription'] != '':
                data = data.filter(location_desc = fb.data['LocationDescription'])     
            # print(fb.data['Description'])
            print(fb.data.get('LocationDescription'))
            if fb.data['Description'] != '':
                data = data.filter(description = fb.data['Description'])
            Check =  fb.check_t_f()
            if Check:
                data = data.filter(created_by_user = Check)
            year1,year2 = fb.returndate()
            print(year1)
            print(year2)
            if year1 is not None and year2 is not None:
                month1 = year1.strftime('%m')
                month2 = year2.strftime('%m')
                year1 = int(year1.strftime('%Y'))
                year2 = int(year2.strftime('%Y'))
                print(month1,month2,year1,year2)
                data= data.filter(date__year__gte = year1)
                # data= data.filter(date__month__gte = month1)
                data= data.filter(date__year__lte = year2)
                #data= data.filter(date__month__lte = month2)
                print(data.values()[:50])

#                data= data.filter(StartDate__year__gte=year,StartDate__month__gte=month,EndDate__year__lte=year, EndDate__month__lte=month)



            data = data.all()[:30]

            return render(request, 'search.html', {'form': fb , 'data':data})
        else:
            return HttpResponse("Please write correct input")
    else:
        form = SearchForm()
        data = CrimeForm()
    return render(request, 'search.html', {'form': form , 'crime': data, 'latlon': latlon, 'results': results})

def simple(request):
    return render(request, 'simple.html')

def base(request):
    return render(request, 'base.html')
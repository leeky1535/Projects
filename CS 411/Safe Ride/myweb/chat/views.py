from django.shortcuts import render,redirect
from django.utils.safestring import mark_safe
import json
from django.http import HttpResponse
from home.models import chat_vocab_t
from django.db.models import Count
import mysql.connector

# Create your views here.

def chat(request):
    return render(request, 'chat/chat.html', {})

def room(request, room_name):
    data = chat_vocab_t.objects.values('vocab').annotate(frequency=Count('vocab')).order_by('-frequency')[:30]
    return render(request, 'chat/room.html', {
        'room_name_json': mark_safe(json.dumps(room_name)), 'data': data
    })


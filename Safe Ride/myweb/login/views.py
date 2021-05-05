from django.shortcuts import render, redirect
from django.http import HttpResponse
from home.models import crime_t
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth import authenticate, login as n_login
from django.contrib.auth.models import User
from login.forms import SignUpForm, UserForm, ProfileForm
from django.contrib.auth.decorators import login_required
from django.db import transaction
from django.utils.translation import ugettext_lazy as _
from django.contrib import messages

# Create your views here.

def login(request):
    return render(request, 'login.html')

def signup(request):
    if request.method == 'POST':
        form = SignUpForm(request.POST)
        if form.is_valid():
            form.save()
            username = form.cleaned_data.get('username')
            raw_password = form.cleaned_data.get('password1')
            user = authenticate(username=username, password=raw_password)
            n_login(request, user)
            return redirect('/login')
    else:
        form = SignUpForm()
    return render(request, 'signup.html', {'form': form})


def reset(request):
    return render(request, 'reset.html')

def plzlogin(request):
    return render(request, 'plzlogin.html')

def update_profile(request):
    if request.method == 'POST':
        user_form = UserForm(request.POST, instance=request.user)
        profile_form = ProfileForm(request.POST, instance=request.user.profile)
        if user_form.is_valid() and profile_form.is_valid():
            user_form.save()
            profile_form.save()
            messages.success(request, _('Your profile was successfully updated!'))
            return redirect('settings:profile')
        else:
            messages.error(request, _('Please correct the error below.'))
    else:
        user_form = UserForm(instance=request.user)
        profile_form = ProfileForm(instance=request.user.profile)
    return render(request, 'profile.html', {
        'user_form': user_form,
        'profile_form': profile_form
    })

def showmylist(request):
    C = crime_t.objects.all()
    user = request.user
    data = C.filter(created_by_user = user.username)
    return render(request, 'mylistview.html', {'data':data})

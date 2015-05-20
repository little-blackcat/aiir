from django.shortcuts import render, render_to_response
from django.http import HttpResponse, Http404, HttpResponseRedirect
from django.template import RequestContext, loader
from .models import Done, Pending
from .forms import RequestForm, TaskForm, UserForm
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required

@login_required
def user_logout(request):
    logout(request)
    return HttpResponseRedirect('/index/')

def user_login(request):
    context = RequestContext(request)
    if request.method == 'POST':
        username = request.POST['username']
        password = request.POST['password']

        user = authenticate(username=username, password=password)

        if user:
            if user.is_active:
                login(request, user) 
                return HttpResponseRedirect('/index/tasks/')
                #return render(request, 'aiir_app/tasks.html', {})
            else:
                return HttpResponse("Twoje konto jest nieaktywne.")
        else:
            print "Nieprawidlowe dane konta: {0}, {1}".format(username, password)
            return HttpResponse("Invalid login details supplied.")
    else:
        return render_to_response('index.html', {}, context)

def register(request):
    context = RequestContext(request)

    registered = False
    if request.method == 'POST':
        user_form = UserForm(data=request.POST)

        if user_form.is_valid():
            user = user_form.save()
            user.set_password(user.password)
            user.save()
            registered = True
        else:
            print user_form.errors
    else:
        user_form = UserForm()

    return render_to_response(
            'aiir_app/register.html',
            {'user_form': user_form, 'registered': registered},
            context)

def index(request):
	return render_to_response('index.html', {'user':request.user})

@login_required
def get_task(request):
	current_result = []
	lista = Pending.objects.filter(current_user=str(request.user.username))
	msg = ''
	all_result = Done.objects.filter(current_user=str(request.user.username))

	if request.method == 'POST':
		form = TaskForm(request.POST)
		if form.is_valid():
			temp = request.POST['range_of']

			try:
				current_result = Done.objects.get(rangeof=temp, current_user=str(request.user.username))
				all_result = Done.objects.all()
				#return HttpResponse("Ilosc liczb pierwszych dla zadanego przedzialu: "+ str(current_result.result))
				msg = "Ilosc liczb pierwszych dla zadanego przedzialu: "+ str(current_result.result)
			except Done.DoesNotExist:
				try:
					x = Pending.objects.get(rangeof=temp, current_user=str(request.user.username))
					msg = "Takie zadanie oczekuje juz w kolejce."
				except Pending.DoesNotExist:
					Pending.objects.create(rangeof=temp,current_user=str(request.user.username))
				#for item in lista:
				#	print "Przedzial: " + str(item.rangeof) + " |\tPriorytet: " + str(item.priority)
				#return HttpResponse("Twoje zadanie zostalo dodane do kolejki.")
					msg = "Twoje zadanie zostalo dodane do kolejki." 
	else:
		form = TaskForm()

	return render(request, 'tasks.html', {'form': form, 'msg': msg, 'current_result': current_result, 'lista': lista, 'all_result': all_result})


from django.shortcuts import render, render_to_response
from django.http import HttpResponse, Http404
from django.template import RequestContext, loader
from .models import Done, Pending
from .forms import RequestForm, TaskForm, UserForm, UserProfileForm

# Create your views here.
def register(request):
    context = RequestContext(request)
    registered = False

    if request.method == 'POST':
        user_form = UserForm(data=request.POST)
        profile_form = UserProfileForm(data=request.POST)

        if user_form.is_valid() and profile_form.is_valid():
            user = user_form.save()
            user.set_password(user.password)
            user.save()

            profile = profile_form.save(commit=False)
            profile.user = user

            if 'picture' in request.FILES:
                profile.picture = request.FILES['picture']

            profile.save()
            registered = True

        else:
            print user_form.errors, profile_form.errors

    else:
        user_form = UserForm()
        profile_form = UserProfileForm()

    return render_to_response(
            'aiir_app/register.html',
            {'user_form': user_form, 'profile_form': profile_form, 'registered': registered},
            context)

def index(request):
	
	return HttpResponse(" ")

def get_task(request):
	current_result = []
	lista = Pending.objects.all()
	msg = ''
	all_result = Done.objects.all()

	if request.method == 'POST':
		form = TaskForm(request.POST)
		if form.is_valid():
			temp = request.POST['range_of']

			try:
				current_result = Done.objects.get(rangeof=temp)
				all_result = Done.objects.all()
				#return HttpResponse("Ilosc liczb pierwszych dla zadanego przedzialu: "+ str(current_result.result))
				msg = "Ilosc liczb pierwszych dla zadanego przedzialu: "+ str(current_result.result)
			except Done.DoesNotExist:
				try:
					x = Pending.objects.get(rangeof=temp)
					msg = "Takie zadanie oczekuje juz w kolejce."
				except Pending.DoesNotExist:
					Pending.objects.create(rangeof=temp)
				#for item in lista:
				#	print "Przedzial: " + str(item.rangeof) + " |\tPriorytet: " + str(item.priority)
				#return HttpResponse("Twoje zadanie zostalo dodane do kolejki.")
					msg = "Twoje zadanie zostalo dodane do kolejki."
	else:
		form = TaskForm()



	return render(request, 'tasks.html', {'form': form, 'msg': msg, 'current_result': current_result, 'lista': lista, 'all_result': all_result})


def detail(request, question_id):
	return HttpResponse("Response")

def results(request, question_id):
    response = "You're looking at the results of question %s."
    return HttpResponse(response % question_id)

def vote(request, question_id):
    return HttpResponse("You're voting on question %s." % question_id)
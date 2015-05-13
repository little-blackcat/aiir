from django import forms
from .models import Pending, UserProfile
from django.contrib.auth.models import User

class RequestForm(forms.Form):
	class Meta:
		model = Pending
		fields = ('rangeof','priority')

class TaskForm(forms.Form):
	range_of = forms.IntegerField(label='Range', min_value=2)

class UserForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta:
        model = User
        fields = ('username', 'email', 'password')

class UserProfileForm(forms.ModelForm):
    class Meta:
        model = UserProfile
        fields = ('website', 'picture')
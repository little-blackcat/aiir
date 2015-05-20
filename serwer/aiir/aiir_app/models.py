from django.db import models
from datetime import datetime
from django.contrib.auth.models import *

# Create your models here.

class Pending(models.Model):
	rangeof = models.IntegerField()
	request_time = models.DateTimeField(default=datetime.now, blank=True, editable=False)
	priority = models.IntegerField(default=1)
	current_user = models.TextField(default='x')

class Done(models.Model):
	rangeof = models.IntegerField()
	result = models.IntegerField()
	current_user = models.TextField(default='x')


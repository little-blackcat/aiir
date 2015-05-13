from django.db import models
from datetime import datetime
from django.contrib.auth.models import User

# Create your models here.

class Pending(models.Model):
	rangeof = models.IntegerField()
	request_time = models.DateTimeField(default=datetime.now, blank=True, editable=False)
	priority = models.IntegerField(default=1)

class Done(models.Model):
	rangeof = models.IntegerField()
	result = models.IntegerField()

class UserProfile(models.Model):
	user = models.OneToOneField(User)

	website = models.URLField(blank=True)
	picture = models.ImageField(upload_to='profile_images', blank=True)

	def __unicode__(self):
		return self.user.username
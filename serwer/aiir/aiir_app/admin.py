from django.contrib import admin

from .models import Pending, Done, UserProfile

class PendingAdmin(admin.ModelAdmin):
	fieldsets = [
		(None, 				{'fields': ['priority']}),
		('Information',		{'fields': ['rangeof']}),
	]
	list_display = ('priority','rangeof','request_time')
	ordering = ('-priority',) #sortowanie wg priorytetow

admin.site.register(Pending, PendingAdmin)
admin.site.register(UserProfile)
from django.contrib import admin

from .models import Pending, Done

class PendingAdmin(admin.ModelAdmin):
	fieldsets = [
		(None, 				{'fields': ['priority']}),
		('Information',		{'fields': ['rangeof']}),
	]
	list_display = ('priority','rangeof','current_user','request_time')
	ordering = ('-priority',) #sortowanie wg priorytetow

admin.site.register(Pending, PendingAdmin)
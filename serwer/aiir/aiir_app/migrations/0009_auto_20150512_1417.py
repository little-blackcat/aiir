# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0008_auto_20150512_1415'),
    ]

    operations = [
        migrations.AlterField(
            model_name='query',
            name='request_time',
            field=models.DateTimeField(default=datetime.datetime.now, editable=False, blank=True),
        ),
    ]

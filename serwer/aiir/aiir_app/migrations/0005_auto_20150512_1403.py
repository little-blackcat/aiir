# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0004_auto_20150512_1346'),
    ]

    operations = [
        migrations.AlterField(
            model_name='query',
            name='request_time',
            field=models.DateTimeField(auto_now_add=True),
        ),
    ]

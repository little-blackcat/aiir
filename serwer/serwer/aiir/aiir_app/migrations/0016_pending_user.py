# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0015_auto_20150516_1631'),
    ]

    operations = [
        migrations.AddField(
            model_name='pending',
            name='user',
            field=models.TextField(default=b'x'),
        ),
    ]

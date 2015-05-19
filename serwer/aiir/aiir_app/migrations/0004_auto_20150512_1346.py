# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0003_auto_20150512_1331'),
    ]

    operations = [
        migrations.AddField(
            model_name='query',
            name='priority',
            field=models.IntegerField(default=1),
        ),
        migrations.AlterField(
            model_name='query',
            name='rangeof',
            field=models.IntegerField(max_length=5),
        ),
    ]

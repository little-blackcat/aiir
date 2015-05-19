# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0007_auto_20150512_1411'),
    ]

    operations = [
        migrations.AlterField(
            model_name='query',
            name='rangeof',
            field=models.IntegerField(),
        ),
    ]

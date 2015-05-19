# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='query',
            name='request_time',
            field=models.DateTimeField(default=datetime.datetime(2015, 5, 11, 14, 5, 26, 919292, tzinfo=utc)),
            preserve_default=False,
        ),
    ]

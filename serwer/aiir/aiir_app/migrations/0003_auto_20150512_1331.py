# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('aiir_app', '0002_query_request_time'),
    ]

    operations = [
        migrations.RenameField(
            model_name='query',
            old_name='range',
            new_name='rangeof',
        ),
    ]

# Generated by Django 2.0.7 on 2018-07-30 13:19

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('home', '0010_auto_20180729_0613'),
    ]

    operations = [
        migrations.AlterField(
            model_name='crime_t',
            name='latitude',
            field=models.FloatField(db_index=True, null=True),
        ),
        migrations.AlterField(
            model_name='crime_t',
            name='longitude',
            field=models.FloatField(db_index=True, null=True),
        ),
    ]

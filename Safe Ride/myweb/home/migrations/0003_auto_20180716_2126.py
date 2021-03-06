# Generated by Django 2.0.7 on 2018-07-16 21:26

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('home', '0002_auto_20180715_2233'),
    ]

    operations = [
        migrations.CreateModel(
            name='crime_t',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('date', models.DateTimeField(auto_now_add=True)),
                ('address', models.CharField(max_length=50)),
                ('latitude', models.FloatField(null=True)),
                ('longtitude', models.FloatField(null=True)),
                ('description', models.TextField()),
                ('location_desc', models.CharField(max_length=30)),
                ('arrest', models.CharField(max_length=30, null=True)),
                ('beat', models.IntegerField(null=True)),
                ('updated_time', models.DateTimeField(null=True)),
                ('violations', models.IntegerField(null=True)),
                ('intersection', models.CharField(max_length=50, null=True)),
                ('created_by_user', models.BooleanField(default=False)),
            ],
        ),
        migrations.DeleteModel(
            name='crime',
        ),
    ]

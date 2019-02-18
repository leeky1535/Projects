# Generated by Django 2.0.7 on 2018-07-26 21:15

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('home', '0005_auto_20180722_2328'),
    ]

    operations = [
        migrations.CreateModel(
            name='chat_vocab_t',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('vocab', models.CharField(max_length=30)),
                ('date', models.DateTimeField(auto_now_add=True)),
                ('side', models.CharField(max_length=30)),
            ],
        ),
        migrations.AlterField(
            model_name='crime_t',
            name='created_by_user',
            field=models.BooleanField(default=False),
        ),
    ]

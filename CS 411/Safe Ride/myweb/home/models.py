from django.db import models
# Create your models here.
class crime_t(models.Model):
    date = models.DateTimeField(auto_now_add=True)
    address = models.CharField(max_length=50)
    latitude = models.FloatField(null=True, db_index=True)
    longitude = models.FloatField(null=True, db_index=True)
    description = models.TextField()
    location_desc = models.CharField(max_length=30)
    arrest = models.CharField(max_length=30, null=True)
    beat = models.IntegerField(null=True)
    updated_time = models.DateTimeField(null=True)
    violations = models.IntegerField(null=True)
    intersection = models.CharField(max_length=50, null=True)
    created_by_user = models.CharField(max_length=30, null=True)

class chat_vocab_t(models.Model):
    vocab = models.CharField(max_length=50, null=False)
    date = models.DateTimeField(auto_now_add=True)
    side = models.CharField(max_length=30, null=True)
    username = models.CharField(max_length=30, null=True)


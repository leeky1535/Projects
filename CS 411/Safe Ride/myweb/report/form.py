from django.forms import ModelForm, Form
from home.models import crime_t
from django import forms
from django.forms.widgets import SelectDateWidget
import datetime




class CrimeForm(ModelForm):
    class Meta:
        model = crime_t
        fields = ['description','location_desc','address']

    option_description= crime_t.objects.values('description').distinct()
    option_location_desc= crime_t.objects.values('location_desc').distinct()
    option_date= crime_t.objects.values('date').distinct()

# class SearchForm(forms.Form):


#     MY_CHOICES = (
#     ('residence', 'residence'),
#     ('2', 'Option 2'),
#     ('', 'ddd')
# 	)
#     LocationDescription = forms.ChoiceField(choices=MY_CHOICES,required = False)
#     Description = forms.ChoiceField(choices=MY_CHOICES,required = False)
#     Date = forms.DateTimeField(required = False)
#     created_by_user = forms.BooleanField(required=False,initial = False)
#     def check_t_f(self):
#         return self.cleaned_data['created_by_user']
# def unique_values():
#     return [("", "---------")] + list(ReadOnlyTable.objects.values_list('pie', 'pie').distinct())

def description_unique():
    return [("", "---------")] + list(crime_t.objects.values_list('description', 'description').distinct().order_by('description'))
def location_desc_unique():
    return [("", "---------")] + list(crime_t.objects.values_list('location_desc', 'location_desc').distinct().order_by('location_desc'))


class SearchForm(forms.Form):

    LocationDescription = forms.MultipleChoiceField(required=False,choices=location_desc_unique)
    Description = forms.MultipleChoiceField(required=False,choices=description_unique)
    StartDate = forms.DateTimeField(required=False, widget=SelectDateWidget(years=range(2012, 2018)))
    EndDate = forms.DateTimeField(required=False,widget=SelectDateWidget(years=range(2012, 2018)))

    created_by_user = forms.BooleanField(required=False,initial = False)
    def check_t_f(self):
        return self.cleaned_data['created_by_user']
    def returndate(self):
        return self.cleaned_data['StartDate'],self.cleaned_data['EndDate']
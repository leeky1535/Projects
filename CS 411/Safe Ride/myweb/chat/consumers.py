from channels.generic.websocket import AsyncWebsocketConsumer
from django.contrib.auth.models import User
from django.shortcuts import render
from django.template import Context
from home.models import chat_vocab_t
import json
import nltk
from nltk.corpus import stopwords, wordnet
from nltk.tokenize import word_tokenize
import datetime

class ChatConsumer(AsyncWebsocketConsumer):
    stop_words = set(stopwords.words('english'))
    def breakdown_store(self, message):
        word_tokens = word_tokenize(message)
        filtered_message = [w for w in word_tokens if not w in self.stop_words and wordnet.synsets(w)]
        now = datetime.datetime.now()
        date_time = now.strftime('%Y-%m-%d')
        for word in filtered_message:
            p = chat_vocab_t(vocab=word, date=date_time, side=self.scope['url_route']['kwargs']['room_name'], username=self.scope["user"])
            p.save()
        
    async def connect(self):
        self.room_name = self.scope['url_route']['kwargs']['room_name']
        self.room_group_name = 'chat_%s' % self.room_name

        # Join room group
        await self.channel_layer.group_add(
            self.room_group_name,
            self.channel_name
        )
        await self.accept()

    async def disconnect(self, close_code):
        # Leave room group
        await self.channel_layer.group_discard(
            self.room_group_name,
            self.channel_name
        )

    # Receive message from WebSocket
    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        message = text_data_json['message']
        user_name = text_data_json['username']
        self.breakdown_store(message)
        # Send message to room group
        await self.channel_layer.group_send(
            self.room_group_name,
            {
                'type': 'chat_message',
                'message': message,
                'username': user_name,
            }
        )

    # Receive message from room group
    async def chat_message(self, event):
        self.user = self.scope["user"]
        user_name = event['username']
        message = event['message']

        # Send message to WebSocket
        await self.send(text_data=json.dumps({
            'message': message,
            'username': user_name,
        }))


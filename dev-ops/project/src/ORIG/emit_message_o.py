#!/usr/bin/env python
import pika
import time
import requests

URL_STATE = "http://api:8083/state"

connectionUp = False
while (not connectionUp):
    try:
        connection = pika.BlockingConnection(
            pika.ConnectionParameters('rabbitmq'))
        connectionUp = True
    except Exception:
        time.sleep(5)

time.sleep(5)  # Making sure other scripts have the connection up for listening

channel = connection.channel()
channel.exchange_declare(exchange='direct_messages', exchange_type='direct')

TOPIC_ORIG = 'compse140.o'
TOPIC_STATE = 'compse140.state'
message = 'MSG_1'  # 'message'
message_num = 1

state = 'INIT'
old_state = 'INIT'  # Old state to track state changes

while state != 'SHUTDOWN':
    connection_errors = 0
    try:
        state = requests.get(URL_STATE).content.decode()
    except ConnectionError:
        if (connection_errors < 1):
            state = 'INIT'
        else:
            state = 'SHUTDOWN'

    if (state == 'INIT' or state == 'RUNNING'):
        message = "MSG_%r" % (message_num)
        channel.basic_publish(
            exchange='direct_messages', routing_key=TOPIC_ORIG, body=message)
        if (state == 'INIT'):
            requests.put(url=URL_STATE, data='RUNNING')
        message_num = message_num + 1
        time.sleep(3)
    else:
        time.sleep(1)  # Limit the amount of requests to the API

channel.basic_publish(
            exchange='direct_messages', routing_key=TOPIC_STATE, body='SHUTDOWN')
connection.close()

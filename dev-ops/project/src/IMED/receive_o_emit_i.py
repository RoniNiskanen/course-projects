#!/usr/bin/env python
import pika
import time
import os

connectionUp = False
while (not connectionUp):
    try:
        connection = pika.BlockingConnection(
            pika.ConnectionParameters('rabbitmq'))
        connectionUp = True
    except Exception:
        time.sleep(1)

channel = connection.channel()
channel.exchange_declare(exchange='direct_messages', exchange_type='direct')

result = channel.queue_declare(queue='', exclusive=True)
queue_name = result.method.queue

channel.queue_bind(
    exchange='direct_messages', queue=queue_name, routing_key='compse140.o')
channel.queue_bind(
    exchange='direct_messages', queue=queue_name, routing_key='compse140.state')

print(' [*] Waiting for logs. To exit press CTRL+C')

channelSend = connection.channel()
channelSend.exchange_declare(exchange='direct_messages', exchange_type='direct')


def callback(ch, method, properties, receivedMessage):
    topic = method.routing_key
    if (topic == 'compse140.o'):
        time.sleep(1)
        message = "Got %r" % (receivedMessage.decode())
        channelSend.basic_publish(
            exchange='direct_messages', routing_key='compse140.i', body=message)
    if (topic == 'compse140.state' and receivedMessage.decode() == 'SHUTDOWN'):
        os._exit(0)


channel.basic_consume(
    queue=queue_name, on_message_callback=callback, auto_ack=True)

channel.start_consuming()

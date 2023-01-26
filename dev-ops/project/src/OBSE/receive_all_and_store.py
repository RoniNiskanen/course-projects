#!/usr/bin/env python
import os
import pika
from datetime import datetime
import time

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
    exchange='direct_messages', queue=queue_name, routing_key='compse140.i')

channel.queue_bind(
    exchange='direct_messages', queue=queue_name, routing_key='compse140.o')

channel.queue_bind(
    exchange='direct_messages', queue=queue_name, routing_key='compse140.state')

print(' [*] Waiting for logs. To exit press CTRL+C')


def callback(ch, method, properties, receivedMessage):
    timestamp = str((datetime.utcnow()))
    topic = method.routing_key
    message_n = 1
    if (topic == 'compse140.i' or topic == 'compse140.o'):
        try:
            file = open('/messages/messages.txt', 'r')
            message_n = sum(1 for line in file) + 1
            file.close()
        except Exception:
            pass

        file = open('/messages/messages.txt', 'a+')
        new_row = timestamp + " " + str(message_n) + " " + str(receivedMessage.decode()) + " to " + str(topic)
        file.write(f"{new_row}\n")
        file.close()

    elif (topic == 'compse140.state' and receivedMessage.decode() == 'SHUTDOWN'):
        os._exit(0)


channel.basic_consume(
    queue=queue_name, on_message_callback=callback, auto_ack=True)

channel.start_consuming()

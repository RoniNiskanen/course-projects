from datetime import datetime
from multiprocessing import Process
from flask import Flask, request
from pymemcache.client import base
import requests
import time
import os

URL_HTTPSERV = "http://httpserv:8080"
URL_QUEUE_NAMES = 'http://rabbitmq_broker:15672/api/queues/'
URL_MONITORING = "http://monitoring:8085"
URL_QUEUE_STATISTIC = 'http://rabbitmq_broker:15672/api/queues/%2F/'
URL_NODE = 'http://rabbitmq_broker:15672/api/nodes'


def API_gateway():
    app = Flask(__name__)

    def increase_request_amount():
        requests_amount = int(client.get('requests_amount'))
        new_requests_amount = str(requests_amount + 1)
        client.set('requests_amount', new_requests_amount)

    @app.route('/requests_amount', methods=['GET'])
    def return_requests_amount():
        increase_request_amount()
        requests_amount = client.get('requests_amount')
        return requests_amount, 200, {'Content-Type': 'text/plain'}

    @app.route('/messages', methods=['GET'])
    def return_messages_from_httpserv():
        response_httpserv = requests.get(URL_HTTPSERV)
        increase_request_amount()
        return response_httpserv.text, 200, {'Content-Type': 'text/plain'}

    @app.route('/node-statistic', methods=['GET'])
    def return_node_statistic():
        response = requests.get(URL_NODE, auth=('guest', 'guest'))
        json_response = response.json()

        name = json_response[0]['name']
        uptime = json_response[0]['uptime']
        disk_free = json_response[0]['disk_free']
        mem_used = json_response[0]['mem_used']
        channel_created = json_response[0]['channel_created']

        node_statistics = {"name": name, "uptime": uptime,
                           "disk_free": disk_free, "mem_used": mem_used, "channel_created": channel_created}
        increase_request_amount()
        return node_statistics, 200, {'Content-Type': 'application/json'}

    @app.route('/queue-statistic', methods=['GET'])
    def return_queue_statistic():
        response = requests.get(URL_QUEUE_NAMES, auth=('guest', 'guest'))
        queue_names = [q['name'] for q in response.json()]

        queue_statistics_all = []
        for name in queue_names:
            url_queue = URL_QUEUE_STATISTIC + name
            response = requests.get(url_queue, auth=('guest', 'guest'))
            json_response = response.json()
            message_delivery_rate = json_response['message_stats']['deliver_get_details']['rate']
            messages_publishing_rate = json_response['message_stats']['publish_details']['rate']
            messages_delivered_recently = json_response['message_stats']['deliver_get']
            message_published_lately = json_response['message_stats']['publish']
            queue_statistics = {"message_delivery_rate": message_delivery_rate, "messages_publishing_rate": messages_publishing_rate,
                                "messages_delivered_recently": messages_delivered_recently,
                                "message_published_lately": message_published_lately}
            queue_statistics_all.append(queue_statistics)
        increase_request_amount()
        return queue_statistics_all, 200, {'Content-Type': 'application/json'}

    @app.route('/run-log', methods=['GET'])
    def return_run_log():
        file = open('/messages/run-log.txt')
        data = file.read()
        increase_request_amount()
        return data, 200, {'Content-Type': 'text/plain'}

    @app.route('/state', methods=['GET', 'PUT'])
    def return_or_set_state():
        if request.method == 'GET':
            state = client.get('state').decode()
            return state, 200, {'Content-Type': 'text/plain'}

        elif request.method == 'PUT':
            new_state = request.data.decode()
            old_state = client.get('state')
            timestamp = str((datetime.utcnow()))

            if (new_state in ['RUNNING', 'PAUSED', 'INIT', 'SHUTDOWN'] and (new_state != old_state)):
                client.set('state', new_state)
                file = open('/messages/run-log.txt', 'a+')
                new_row = timestamp + ": " + new_state
                file.write(f"{new_row}\n")
                file.close()
            if (new_state == 'SHUTDOWN'):
                requests.post(URL_HTTPSERV)
            return new_state, 200, {'Content-Type': 'text/plain'}

    app.run(host='0.0.0.0', port=8083)


if __name__ == '__main__':
    try:
        os.remove('/messages/messages.txt')
    except FileNotFoundError:
        print("messages.text not found")
    try:
        os.remove('/messages/run-log.txt')
    except FileNotFoundError:
        print("run-log.txt not found")

    client = base.Client(('127.0.0.1', 11211))
    client.set('state', 'INIT')
    client.set('requests_amount', '0')
    API_process = Process(target=API_gateway)
    API_process.start()
    running = True
    while running:
        time.sleep(2)
        if (client.get('state').decode() == 'SHUTDOWN'):
            API_process.terminate()
            API_process.join()
            running = False
            requests.post(URL_MONITORING)
    os._exit(0)

from http.server import BaseHTTPRequestHandler, HTTPServer
import requests
from datetime import datetime
import os

hostName = ''
serverPort = 8085
API_URL_REQUESTS_AMOUNT = 'http://api:8083/requests_amount'


class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        amount_of_requests = requests.get(API_URL_REQUESTS_AMOUNT).text
        self.send_response(200)
        self.send_header("Content-type", "text/plain")
        self.end_headers()
        amount_of_requests_string = 'Amount of requests to API: ' + str(amount_of_requests) + '\n'
        current_time = datetime.now()
        uptime_string = 'Server uptime: ' + str(current_time - START_TIME) + '\n'
        start_time_string = 'Server start time: ' + str(START_TIME)

        data = amount_of_requests_string + uptime_string + start_time_string
        self.wfile.write(bytes(data, encoding='utf-8'))

    def do_POST(self):
        os._exit(0)


if __name__ == '__main__':
    START_TIME = datetime.now()
    webServer = HTTPServer((hostName, serverPort), MyServer)
    webServer.serve_forever()

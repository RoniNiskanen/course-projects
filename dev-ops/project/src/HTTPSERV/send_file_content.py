from http.server import BaseHTTPRequestHandler, HTTPServer
import os

hostName = ''
serverPort = 8080


def open_file(path):
    file = open(path, 'r')
    return file


class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/plain")
        self.end_headers()
        file = open_file('/messages/messages.txt')
        data = file.read()
        self.wfile.write(bytes(data, encoding='utf-8'))

    def do_POST(self):
        os._exit(0)


if __name__ == '__main__':
    webServer = HTTPServer((hostName, serverPort), MyServer)
    webServer.serve_forever()

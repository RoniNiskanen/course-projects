import requests

URL_STATE = "http://192.168.0.109:8083/state"
URL_MESSAGES = "http://192.168.0.109:8083/messages"
URL_LOG = "http://192.168.0.109:8083/run-log"
URL_HTTPSERV = "http://192.168.0.109:8080"
URL_NODE_STATISTIC = "http://192.168.0.109:8083/node-statistic"
URL_QUEUE_STATISTIC = "http://192.168.0.109:8083/queue-statistic"
URL_MONITORING = "http://192.168.0.109:8085/monitoring"


# Tests for HTTPSERV
def test_HttpServStatusCodeIs200():
    response = requests.get(URL_HTTPSERV)
    assert response.status_code == 200


# Tests for API gateway on port 8083
# Tests for GET /messages

def test_GetMessages_StatusCodeis200():
    response = requests.get(URL_MESSAGES)
    assert response.status_code == 200


def test_GetMessages_ContentTypeIsText():
    response = requests.get(URL_MESSAGES)
    assert response.headers['content-type'] == 'text/plain'


def test_GetMessages_ReturnsSameAsHttpServ():
    response_msg = requests.get(URL_MESSAGES)
    response_httpserv = requests.get(URL_HTTPSERV)
    assert response_msg.content == response_httpserv.content


# Tests for GET /state and PUT /state
def test_ChangeState_ReturnsSetStatePaused():
    requests.put(url=URL_STATE, data='PAUSED')
    response_get = requests.get(URL_STATE).content.decode()
    assert response_get == 'PAUSED'


def test_ChangeState_ReturnsSetStateRunning():
    requests.put(url=URL_STATE, data='RUNNING')
    response_get = requests.get(URL_STATE).content.decode()
    assert response_get == 'RUNNING'


def test_ChangeState_ReturnsSetStateInit():
    requests.put(url=URL_STATE, data='INIT')
    response_get = requests.get(URL_STATE).content.decode()
    assert response_get == 'INIT'

#
# def test_ChangeState_ShutdownServerOnSetStateShutdown():
#    requests.put(url=URL_STATE, data='SHUTDOWN')
#    response_get = requests.get(URL_STATE).content.decode()
#    assert response_get == 'SHUTDOWN'
#


# Tests for /run-log
def test_RunLog_ContentTypeIsText():
    response = requests.get(URL_LOG)
    assert response.headers['content-type'] == 'text/plain'


def test_RunLog_StatusCodeIs200():
    response = requests.get(URL_LOG)
    assert response.status_code == 200


def test_RunLog_HasPausedInLog():
    response = requests.get(URL_LOG).content.decode()
    in_log = response.find('PAUSED')
    assert in_log != -1


def test_RunLog_HasRunningInLog():
    response = requests.get(URL_LOG).content.decode()
    in_log = response.find('RUNNING')
    assert in_log != -1


def test_RunLog_HasInitInLog():
    response = requests.get(URL_LOG).content.decode()
    in_log = response.find('INIT')
    assert in_log != -1


# Tests for /node-statistic
def test_NodeStatistic_ReturnTypeIsJson():
    response = requests.get(URL_NODE_STATISTIC)
    assert response.headers['content-type'] == 'application/json'


def test_NodeStatistic_StatusCodeis200():
    response = requests.get(URL_NODE_STATISTIC)
    assert response.status_code == 200


# Tests for /queue-statistic
def test_QueueStatistic_ReturnTypeIsJson():
    response = requests.get(URL_QUEUE_STATISTIC)
    assert response.headers['content-type'] == 'application/json'


def test_QueueStatistic_StatusCodeis200():
    response = requests.get(URL_QUEUE_STATISTIC)
    assert response.status_code == 200


def test_QueueStatistic_ResponseHasRightItems():
    response_text = requests.get(URL_QUEUE_STATISTIC).text

    substrings = []
    substrings.append('message_delivery_rate')
    substrings.append('message_published_lately')
    substrings.append('messages_delivered_recently')
    substrings.append('messages_publishing_rate')

    substrings_found = True
    for substring in substrings:
        if substring not in response_text:
            substrings_found = False

    assert substrings_found is True


#  Tests for /monitoring
def test_Monitoring_StatusCodeIs200():
    response = requests.get(URL_MONITORING)
    assert response.status_code == 200


def test_Monitoring_ReturnTypeIsText():
    response = requests.get(URL_MONITORING)
    assert response.headers['content-type'] == 'text/plain'


def test_Monitoring_ResponseHasRightRows():
    response_text = requests.get(URL_MONITORING).text

    substrings = []
    substrings.append('Amount of requests to API')
    substrings.append('Server uptime')
    substrings.append('Server start time')

    substrings_found = True
    for substring in substrings:
        if substring not in response_text:
            substrings_found = False

    assert substrings_found is True

import os
from HTTPSERV.send_file_content import open_file


def test_open_file_returns_file():
    try:
        test_file_path = './test.txt'
        f = open(test_file_path, 'x')
        f.close()
        file = open_file(test_file_path)
        assert file.closed is False
    except Exception:
        assert False
    finally:
        file.close()
        os.remove(test_file_path)

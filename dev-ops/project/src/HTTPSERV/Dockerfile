FROM debian
USER root
RUN apt update
RUN apt install -y python3
RUN apt install -y python3-pip
RUN pip install requests
COPY send_file_content.py .

ENTRYPOINT python3 send_file_content.py
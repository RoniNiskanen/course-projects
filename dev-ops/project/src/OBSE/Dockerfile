FROM debian
USER root
RUN apt update
RUN apt install -y python3
RUN apt install -y python3-pip
RUN pip install pika
COPY receive_all_and_store.py .

ENTRYPOINT python3 receive_all_and_store.py
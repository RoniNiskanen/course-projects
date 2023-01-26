FROM debian
USER root
RUN apt update
RUN apt install -y python3
RUN apt install -y python3-pip
RUN pip install pika
RUN pip install Flask
RUN pip install requests
COPY emit_message_o.py .

ENTRYPOINT python3 emit_message_o.py
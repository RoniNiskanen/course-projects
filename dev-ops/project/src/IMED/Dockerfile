FROM debian
USER root
RUN apt update
RUN apt install -y python3
RUN apt install -y python3-pip
RUN pip install pika
COPY receive_o_emit_i.py .

ENTRYPOINT python3 receive_o_emit_i.py
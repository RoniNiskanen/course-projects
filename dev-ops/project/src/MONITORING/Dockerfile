FROM debian
USER root
RUN apt update
RUN apt install -y python3
RUN apt install -y python3-pip
RUN pip install requests
COPY monitoring.py .

ENTRYPOINT python3 monitoring.py
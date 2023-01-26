FROM debian
USER root
RUN apt update
RUN apt install -y sudo
RUN apt install -y python3
RUN apt install -y python3-pip
RUN apt install -y memcached libmemcached-tools
RUN pip install Flask
RUN pip install requests
RUN pip install pymemcache

COPY api_gateway.py .

ENTRYPOINT sudo service memcached restart && python3 api_gateway.py
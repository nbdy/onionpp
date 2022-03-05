FROM debian:buster

RUN apt-get update && \
    apt-get install -y python3 python3-dev python3-pybind11 git cmake build-essential sudo \
                       libevent-dev libssl-dev zlib1g-dev libzstd-dev autotools-dev automake \
                       liblzma-dev

RUN git clone https://github.com/nbdy/onionpp

WORKDIR "/onionpp/"
RUN ./build.sh

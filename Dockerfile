FROM alpine:latest AS builder

RUN apk add --no-cache build-base cmake ninja bash git perl libtool autoconf automake libcap-static libevent-dev

WORKDIR /tmp
COPY . /tmp

RUN bash script/build-torpp.sh build

FROM alpine:latest

RUN apk add --no-cache bash

COPY --from=builder /usr/lib/lib*.so* /usr/lib/
COPY --from=builder /tmp/build/libevent/bin/lib/libevent*.so* /usr/lib/

RUN adduser -D torpp
USER torpp

WORKDIR /app
COPY --from=builder /tmp/build/torpp .

ENTRYPOINT ["/app/torpp"]

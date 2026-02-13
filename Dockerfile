# Etap 1: build
FROM ubuntu:22.04 AS builder

RUN apt-get update && \
    apt-get install -y build-essential cmake git && \
    apt-get clean

WORKDIR /app
COPY . /app

RUN mkdir -p build
WORKDIR /app/build

RUN cmake .. && cmake --build . --target AsioServer && cmake --build . --target AsioClient

# Etap 2: minimalny runtime
FROM ubuntu:22.04
WORKDIR /app

COPY --from=builder /app/build/AsioServer .
COPY --from=builder /app/build/AsioClient .

CMD ["./AsioServer"]

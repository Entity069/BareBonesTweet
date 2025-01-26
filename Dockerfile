FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    cmake \
    curl \
    sqlite3 \
    libsqlite3-dev \
    gcc \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /.

COPY . .

RUN g++ main.cpp src/*.cpp -lsqlite3 -o web

EXPOSE 8080

CMD ["./web"]

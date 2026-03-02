# ---- Build stage ----
FROM ubuntu:24.04 AS builder

RUN apt update && \
    apt install g++ -y && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY blackjack.cpp /app/blackjack.cpp
RUN g++ blackjack.cpp -o blackjack


# ---- Runtime stage ----
FROM ubuntu:24.04

WORKDIR /app
COPY --from=builder app/blackjack .

CMD ["./blackjack"]

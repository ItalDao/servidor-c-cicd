FROM gcc:12.2.0

WORKDIR /app

COPY server.c .

RUN gcc -o server server.c

EXPOSE 8080

CMD ["./server"]



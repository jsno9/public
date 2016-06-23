all:server client
.PHONY:server
server:
	gcc server.c -o server -pthread
.PHONY:client
client:
	gcc client.c -o client
clean:
	rm -rf server
	rm -rf client

client_local: client.c errors.c database_queries.c
							gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
							./client 127.0.0.1 8080

client_remote: client.c errors.c database_queries.c
								gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
								./client $(IP) 8080

server: server.c errors.c database_queries.c
				gcc server.c errors.c database_queries.c -o server `mysql_config --cflags --libs`
				./server 8080

clean: client server
				rm -rf client
				rm -rf server

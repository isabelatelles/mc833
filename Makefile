client_local: client.c errors.c database_queries.c
							gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
							./client 127.0.0.1 8080

server: server.c errors.c database_queries.c
				gcc server.c errors.c database_queries.c -o server `mysql_config --cflags --libs`
				./server
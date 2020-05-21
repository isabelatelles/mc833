client_local: client.c errors.c database_queries.c
							gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
							./client 127.0.0.1 $(PORT)

client_remote: client.c errors.c database_queries.c
								gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
								./client $(IP) $(PORT)

server: server.c errors.c database_queries.c
				gcc server.c errors.c database_queries.c -o server `mysql_config --cflags --libs`
				./server $(PORT)

clean: client server
				rm -rf client
				rm -rf server

client_tcp_local: client.c errors.c database_queries.c
									gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
									./client 127.0.0.1 $(PORT)

client_tcp_remote: client.c errors.c database_queries.c
										gcc client.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
										./client $(IP) $(PORT)

server_tcp: server_tcp.c errors.c database_queries.c
						gcc server_tcp.c errors.c database_queries.c -o server `mysql_config --cflags --libs`
						./server $(PORT)

client_udp_local: client_udp.c errors.c database_queries.c
									gcc client_udp.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
									./client 127.0.0.1 $(PORT)

client_udp_remote: client_udp.c errors.c database_queries.c
										gcc client_udp.c errors.c database_queries.c -o client `mysql_config --cflags --libs`
										./client $(IP) $(PORT)

server_udp: server_udp.c errors.c database_queries.c
						gcc server_udp.c errors.c database_queries.c -o server `mysql_config --cflags --libs`
						./server $(PORT)

clean: client server
				rm -rf client
				rm -rf server

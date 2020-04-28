/* ********************************************************************* *
 *
 * TCP Client (implementation)
 * Description: responsible for sending requests to server.
 *
 * Reference: https://www.geeksforgeeks.org/socket-programming-cc/
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho - 187506
 *
 * ********************************************************************* */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "errors.h"
#include "client_server.h"
// #include "unp.h"

int create_socket() {
  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    report_error();
  }
  return socket_fd;
}

int read_integer() {
  int integer;
  scanf("%d", &integer);
  return integer;
}

int main(int argc, char **argv) {
  // if (argc != 2) {
  //   report_error_msg("Usage: ./client <server_ip> <server_port>");
  // }

  struct sockaddr_in serv_addr;

  int socket_fd = create_socket();

  // char *server_addr_str = argv[1];
  // int server_port = atoi(argv[2]);
  char *server_addr_str = "127.0.0.1";
  int server_port = 8080;

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, server_addr_str, &serv_addr.sin_addr) <= 0) {
    report_error();
  }

  if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    report_error();
  }
  printf("Connected to server.\n\n");

  const char *display = "Choose one option:\n"
                        "0 - Register new movie\n"
                        "1 - Remove a movie of ID\n"
                        "2 - List titles and exhibition rooms of all movies\n"
                        "3 - List all movie titles of a GENRE\n"
                        "4 - Return movie title of ID\n"
                        "5 - Return movie informations of ID\n"
                        "6 - List all movies\n\n";
  printf("%s", display);

  int option = read_integer();

  switch(option) {
    case OP_CREATE_MOVIE:
    {
      char title[150];
      scanf(" %[^\n]s", title);
      char synopsis[500];
      scanf(" %[^\n]s", synopsis);
      char genre[45];
      scanf(" %[^\n]s", genre);
    }
      break;
    case OP_REMOVE_MOVIE_ID:
      {
        printf("Please, provide the movie id: ");
        int id = read_integer();
        char send_buffer[sizeof(option) + sizeof(id)];
        memcpy(send_buffer, &option, sizeof(option));
        memcpy(send_buffer + sizeof(option), &id, sizeof(id));
        write(socket_fd, send_buffer, sizeof(send_buffer));
      }
      break;
    case OP_GET_EXHIBITION_ROOM:
      break;
    case OP_GET_MOVIE_TITLES_OF_GENRE:
      break;
    case OP_GET_MOVIE_TITLE_OF_ID:
      {
        printf("Please, provide the movie id: ");
        int id = read_integer();
        char send_buffer[sizeof(option) + sizeof(id)];
        memcpy(send_buffer, &option, sizeof(option));
        memcpy(send_buffer + sizeof(option), &id, sizeof(id));
        printf("%d\n", *send_buffer);
        printf("%d\n", *(send_buffer + 4));
        write(socket_fd, send_buffer, sizeof(send_buffer));
        printf("Message sent.\n");
      }
      break;
    case OP_GET_MOVIE_OF_ID:
      {
        printf("Please, provide the movie id: ");
        int id = read_integer();
        char send_buffer[MAX_SIZE];
        memcpy(send_buffer, &option, sizeof(option));
        memcpy(send_buffer + sizeof(option), &id, sizeof(id));
        write(socket_fd, send_buffer, sizeof(send_buffer));

        char recv_buffer[MAX_SIZE];
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        printf("%s\n", recv_buffer);
      }
      break;
    case OP_GET_MOVIES:
      break;
    default:
      report_error_msg("Invalid option chosen.\n");
  }

  return 0;
}

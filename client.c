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
#include <netinet/in.h>
#include <string.h>
#include <errors.h>

int create_socket() {
  int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    report_error();
  }
  return socket_fd;
}

int read_integer() {
  int integer;
  scanf(" %d", &integer);
  return integer;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <server_ip> <server_port>", argv[0]);
    
  }

  struct sockaddr_in serv_addr; 

  int socket_fd = create_socket();

  char *server_addr_str = argv[1];
  int server_port = atoi(argv[2]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, server_addr_str, &serv_addr.sin_addr) <= 0) {
    report_error();
  }

  if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    report_error();
  }
  printf("Connected to server.\n");

  const char *display = "Choose one option:\n"
                        "1 - Register new movie\n"
                        "2 - Remove a movie of ID\n"
                        "3 - List titles and exhibition rooms of all movies\n"
                        "4 - List all movie titles of a GENRE\n"
                        "5 - Return movie title of ID\n"
                        "6 - Return movie informations of ID\n"
                        "7 - List all movies\n\n";
  printf("%s", display);

  int option = read_integer();

  switch(option) {
    case 1:
      char title[150];
      scanf(" %[^\n]s", title);
      char synopsis[500];
      scanf(" %[^\n]s", synopsis);
      char genre[45];
      scanf(" %[^\n]s", genre);

      break;
    case 2:
      {
        int id = read_integer();

      }
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      {
        int id = read_integer();

      }
      break;
    case 6:
      {
        int id = read_integer();

      }
      break;
    case 7:
      break;
    default:
      report_error_msg("Invalid option chosen.\n");
  }

  return 0;
}
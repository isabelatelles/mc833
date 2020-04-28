/* ********************************************************************* *
 *
 * TCP Server (implementation)
 * Description: responsable to provide services to clients
 *
 * Reference: https://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho - 187506
 *
 * ********************************************************************* */

#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "database_queries.h"
#include "errors.h"
#include "commom.h"

#define PORT 8080
#define SA struct sockaddr

int main(int argc, char const *argv[]) {
  int socket_fd, new_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;

  /* Create socket */
  socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    printf("socket creation failed...\n");
    exit(0);
  }
  printf("Socket successfully created..\n");

  /* Configure server settings */
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Bind */
  if ((bind(socket_fd, (SA*) &server_addr, sizeof(server_addr))) != 0) {
    printf("socket bind failed...\n");
    exit(0);
  }
  printf("Socket successfully binded..\n");

  /* Listen */
  if((listen(socket_fd, 5)) != 0){
    printf("Listen failed...\n");
    exit(0);
  }
  printf("Server listening..\n");

  /* Accept clients conections */

  char data[MAX_SIZE];

  while(1){
    addr_size = sizeof(client_addr);
    new_fd = accept(socket_fd, (SA *) &server_vaddr, &addr_size);
    if(!fork()){
      close(socket_fd);
      printf("%s\n", read_message(new_fd));
      int option = 0;

      switch (option) {
        case OP_CREATE_MOVIE:
          break;
        case OP_REMOVE_MOVIE_ID:
          break;
        case OP_GET_EXHIBITION_ROOM:
          break;
        case OP_GET_MOVIE_TITLES_OF_GENRE:
          break;
        case OP_GET_MOVIE_TITLE_OF_ID:
        {
        }
          break;
        case OP_GET_MOVIE_OF_ID:
          break;
        case OP_GET_MOVIES:
          break;
        default:
          report_error_msg("Invalid option chosen.\n");
      }
      /* NOSSO CODIGO DE CONEXAO COM O BD DEVE VIR AQUI */

      close(new_fd);
      exit(1);
    }
    close(new_fd);
  }

  return 0;
}

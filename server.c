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
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 3490
#define SA struct sockaddr 

int main(int argc, char const *argv[]) {
  int socket_fd, new_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;

  /* Create socket */
  socket_fd = Socket(PF_INET, SOCK_STREAM, 0);
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
  while(1){
    addr_size = sizeof(client_addr);
    new_fd = accept(socket_fd, (SA *) &server_addr, &addr_size);
    if(!fork()){
      close(socket_fd);

      /* NOSSO CODIGO DE CONEXAO COM O BD DEVE VIR AQUI */

      close(new_fd);
      exit(1);
    }
    close(new_fd);
  }

  return 0;
}

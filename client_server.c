// #include "unp.h"
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <stdio.h> 
#include "client_server.h"
#include "errors.h"

char* read_message(int socket){
  size_t n;
  char buffer[MAX_SIZE];

again:
  while((n = read(socket, buffer, MAX_SIZE)) > 0);
  // writen(socket, buffer, n);
  if (n < 0 && errno == EINTR){
    goto again;
  } else {
    report_error_msg("Error while receive client request.\n");
  }

  return buffer;
}
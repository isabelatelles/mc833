#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>

#include "unp.h"

#define MAX_SIZE 6000

enum options {
  OP_CREATE_MOVIE,
  OP_REMOVE_MOVIE_ID,
  OP_GET_EXHIBITION_ROOM,
  OP_GET_MOVIE_TITLES_OF_GENRE,
  OP_GET_MOVIE_TITLE_OF_ID,
  OP_GET_MOVIE_OF_ID,
  OP_GET_MOVIES
}

char* read_message(int socket){
  size_t n;
  char buffer[MAX_SIZE];

again:
  n = read(socket, buffer, n));
  if (n>0 && errno == EINTR){
    goto again;
  } else {
    report_error_msg("Error while recive client request.\n");
  }

  return buffer;
}

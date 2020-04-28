#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define MAX_SIZE 6000

enum options {
  OP_CREATE_MOVIE,
  OP_REMOVE_MOVIE_ID,
  OP_GET_EXHIBITION_ROOM,
  OP_GET_MOVIE_TITLES_OF_GENRE,
  OP_GET_MOVIE_TITLE_OF_ID,
  OP_GET_MOVIE_OF_ID,
  OP_GET_MOVIES
};

char* read_message(int);
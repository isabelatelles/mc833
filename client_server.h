#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define MAX_SIZE 6000
#define MAX_SIZE_TITLE 150
#define MAX_SIZE_GENRE 45
#define MAX_SIZE_SYNOPSIS 500

enum options {
  OP_CREATE_MOVIE,
  OP_REMOVE_MOVIE_ID,
  OP_GET_EXHIBITION_ROOM,
  OP_GET_MOVIE_TITLES_OF_GENRE,
  OP_GET_MOVIE_TITLE_OF_ID,
  OP_GET_MOVIE_OF_ID,
  OP_GET_MOVIES
};

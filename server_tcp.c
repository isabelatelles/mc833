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
#include <unistd.h>

#include "database_queries.h"
#include "errors.h"
#include "client_server.h"

#define SA struct sockaddr

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    report_error_msg("Usage: ./server <listening_port>");
  }

  int listening_port = atoi(argv[1]);

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
  server_addr.sin_port = htons(listening_port);
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
    new_fd = accept(socket_fd, (SA *) &server_addr, &addr_size);
    if(!fork()){
      close(socket_fd);

      char buffer[MAX_SIZE];
      int shift = 0;
      recv(new_fd, buffer, sizeof(buffer), MSG_WAITALL);
      enum options option;

      memcpy(&option, buffer, sizeof(option));

      shift += sizeof(option);
      printf("OP: %d\n\n", option);


      switch (option) {
        case OP_CREATE_MOVIE:
        {
          Movie new_movie;
          int room_number, new_movie_id;
          char send_buffer[MAX_SIZE];

          memcpy(&new_movie, &buffer[shift],sizeof(new_movie));
          shift += sizeof(new_movie);
          memcpy(&room_number, &buffer[shift], sizeof(room_number));

          printf("RECEIVED\n");
          printf("title: %s\n", new_movie.title);
          printf("synopsis: %s\n", new_movie.synopsis);
          printf("genre: %s\n\n", new_movie.genre);

          new_movie_id = create_movie(new_movie, room_number);

          printf("SENT\n");
          printf("insered movie id: %d\n", new_movie_id);
          printf("-------------------------------------------------------\n");

          memcpy(&send_buffer[0], &new_movie_id, sizeof(new_movie_id));
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        case OP_REMOVE_MOVIE_ID:
        {
          int movie_id;
          memcpy(&movie_id, &buffer[shift], sizeof(movie_id));

          printf("RECEIVED\n");
          printf("movie id: %d\n\n", movie_id);

          remove_movie_id(movie_id);

          printf("SENT\n");
          printf("success\n");
          printf("-------------------------------------------------------\n");

          char send_buffer[MAX_SIZE];
          int success = 1;
          memcpy(&send_buffer[0], &success, sizeof(success));
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        case OP_GET_EXHIBITION_ROOM:
        {
          ExhibitionRoom * exhibition_rooms;
          int size;

          printf("RECEIVED\n\n");

          exhibition_rooms = get_exhibition_rooms(&size);

          printf("SENT\n");
          for (int i = 0; i < size; i++){
            printf("room number: %d\n", exhibition_rooms[i].room_number);
            printf("movie title: %s\n\n", exhibition_rooms[i].movie_title);
          }
          printf("-------------------------------------------------------\n");

          char send_buffer[MAX_SIZE];
          memcpy(&send_buffer[0], &size, sizeof(size));
          memcpy(&send_buffer[sizeof(size)], exhibition_rooms, sizeof(ExhibitionRoom)*size);
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        case OP_GET_MOVIE_TITLES_OF_GENRE:
        {
          char genre[MAX_SIZE_GENRE];
          memcpy(genre, &buffer[shift], sizeof(genre));

          printf("RECEIVED\n");
          printf("genre: %s\n\n", genre);

          int size;
          char * titles;
          titles = get_movie_titles_of_genre(genre, &size);

          printf("SENT\n");
          for (int i = 0; i < size; i++) {
            printf("title: %s\n", &titles[i * MAX_SIZE_TITLE]);
          }
          printf("-------------------------------------------------------\n");

          char send_buffer[MAX_SIZE];
          memcpy(&send_buffer[0], &size, sizeof(size));
          memcpy(&send_buffer[sizeof(size)], titles, sizeof(char) * MAX_SIZE_TITLE * size);
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        case OP_GET_MOVIE_TITLE_OF_ID:
        {
          int movie_id;
          char * title;
          memcpy(&movie_id, &buffer[shift], sizeof(movie_id));

          printf("RECEIVED\n");
          printf("movie id: %d\n\n", movie_id);

          title =  get_movie_title_of_id(movie_id);

          printf("SENT\n");
          printf("movie title: %s\n", title);
          printf("-------------------------------------------------------\n");

          char send_buffer[MAX_SIZE];

          memcpy(&send_buffer[0], title, strlen(title));
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        case OP_GET_MOVIE_OF_ID:
        {
          int movie_id;
          char send_buffer[MAX_SIZE];
          Movie movie;


          memcpy(&movie_id, &buffer[shift],sizeof(movie_id));
          printf("RECEIVED\n");
          printf("movie id: %d\n\n", movie_id);

          movie = get_movie_of_id(movie_id);

          printf("SENT\n");
          printf("movie id: %d\n", movie.id);
          printf("title: %s\n", movie.title);
          printf("synopsis: %s\n", movie.synopsis);
          printf("genre: %s\n", movie.genre);
          printf("-------------------------------------------------------\n");

          memcpy(&send_buffer, &movie, sizeof(movie));
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        case OP_GET_MOVIES:
        {
          Movie* movies;
          int size;

          printf("RECEIVED\n\n");

          movies = get_movies(&size);

          printf("SENT\n");

          for (int i = 0; i < size; i++){
            printf("movie id: %d\n", movies[i].id);
            printf("title: %s\n", movies[i].title);
            printf("synopsis: %s\n", movies[i].synopsis);
            printf("genre: %s\n\n", movies[i].genre);
          }
          printf("-------------------------------------------------------\n");

          char send_buffer[MAX_SIZE];
          memcpy(&send_buffer[0], &size, sizeof(size));
          memcpy(&send_buffer[sizeof(size)], movies, sizeof(Movie)*size);
          write(new_fd, send_buffer, sizeof(send_buffer));

          break;
        }
        default:
          report_error_msg("Invalid option chosen.\n");
      }

      close(new_fd);
      exit(1);
    }

    close(new_fd);
  }

  return 0;
}

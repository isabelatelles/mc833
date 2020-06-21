/* ********************************************************************* *
 *
 * UDP Client (implementation)
 * Description: responsible for sending requests to server.
 *
 * Reference: https://www.geeksforgeeks.org/udp-client-server-using-connect-c-implementation/
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
#include "database_queries.h"
// #include "unp.h"

int create_socket() {
  int socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
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

void send_buffer_with_option(int socket_fd, const SA *p_serv_addr, socklen_t serv_len,
  int option) {
  char send_buffer[MAX_SIZE];
  memcpy(send_buffer, &option, sizeof(option));
  if ((sendto(socket_fd, send_buffer, sizeof(send_buffer), 0, p_serv_addr, serv_len)) < 0) {
    report_error();
  }
}

void send_buffer_with_option_and_id(int socket_fd, const SA *p_serv_addr,
  socklen_t serv_len, int option, int id) {
  char send_buffer[MAX_SIZE];
  memcpy(send_buffer, &option, sizeof(option));
  memcpy(send_buffer + sizeof(option), &id, sizeof(id));
  if ((sendto(socket_fd, send_buffer, sizeof(send_buffer), 0, p_serv_addr, serv_len)) < 0) {
    report_error();
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    report_error_msg("Usage: ./client <server_ip> <server_port>");
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
  printf("Connected to server.\n\n");

  const char *display = "Choose one option:\n"
                        "0 - Register new movie\n"
                        "1 - Remove a movie of ID\n"
                        "2 - List titles and exhibition rooms of all movies\n"
                        "3 - List all movie titles of a GENRE\n"
                        "4 - Return movie title of ID\n"
                        "5 - Return movie informations of ID\n"
                        "6 - List all movies\n"
                        "7 - Close connection\n";
  printf("%s\n", display);

  int option = read_integer();

  while(option != OP_CLOSE_CONNECTION) {
    switch(option) {
      case OP_CREATE_MOVIE:
        {
          Movie movie;
          int room_number;
          printf("Please, provide the movie title: ");
          scanf(" %[^\n]s", movie.title);
          printf("Please, provide the movie synopsis: ");
          scanf(" %[^\n]s", movie.synopsis);
          printf("Please, provide the movie genre: ");
          scanf(" %[^\n]s", movie.genre);
          printf("Please, provide the exhibition room number: ");
          room_number = read_integer();

          char send_buffer[MAX_SIZE];
          memcpy(send_buffer, &option, sizeof(option));
          memcpy(&send_buffer[sizeof(option)], &movie, sizeof(movie));
          memcpy(&send_buffer[sizeof(option) + sizeof(movie)], &room_number, sizeof(room_number));
          if (sendto(socket_fd, send_buffer, sizeof(send_buffer), 0, (SA*) &serv_addr, sizeof(serv_addr)) < 0) {
            report_error();
          }

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          int recv_id;
          memcpy(&recv_id, recv_buffer, sizeof(recv_id));

          printf("Movie of id %d was successfully created.\n", recv_id);
        }
        break;
      case OP_REMOVE_MOVIE_ID:
        {
          printf("Please, provide the movie id: ");
          int id = read_integer();

          send_buffer_with_option_and_id(socket_fd, (SA*) &serv_addr, sizeof(serv_addr), option, id);

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          int succeeded;
          memcpy(&succeeded, recv_buffer, sizeof(succeeded));

          if (succeeded) {
            printf("Movie of id %d successfully removed.\n", id);
          } else {
            printf("Movie of id %d was not removed.\n", id);
          }
        }
        break;
      case OP_GET_EXHIBITION_ROOM:
        {
          send_buffer_with_option(socket_fd, (SA*) &serv_addr, sizeof(serv_addr), option);

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          int num_recv_exhib_rooms;
          memcpy(&num_recv_exhib_rooms, recv_buffer, sizeof(num_recv_exhib_rooms));

          ExhibitionRoom *recv_exhib_rooms = malloc(sizeof(ExhibitionRoom) * num_recv_exhib_rooms);
          memcpy(recv_exhib_rooms, recv_buffer + sizeof(num_recv_exhib_rooms),
            sizeof(ExhibitionRoom) * num_recv_exhib_rooms);

          for (int i = 0; i < num_recv_exhib_rooms; i++) {
            printf("Room number: %d\n", recv_exhib_rooms[i].room_number);
            printf("Movie title: %s\n\n", recv_exhib_rooms[i].movie_title);
          }

          free(recv_exhib_rooms);
        }
        break;
      case OP_GET_MOVIE_TITLES_OF_GENRE:
        {
          char genre[MAX_SIZE_GENRE];
          printf("Please, provide the movie genre: ");
          scanf(" %[^\n]s", genre);

          char send_buffer[MAX_SIZE];
          memcpy(send_buffer, &option, sizeof(option));
          memcpy(send_buffer + sizeof(option), genre, sizeof(genre));
          if (sendto(socket_fd, send_buffer, sizeof(send_buffer), 0, (SA*) &serv_addr, sizeof(serv_addr)) < 0) {
            report_error();
          }

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          int num_recv_titles;
          memcpy(&num_recv_titles, recv_buffer, sizeof(num_recv_titles));

          char *recv_titles = malloc(sizeof(char) * MAX_SIZE_TITLE * num_recv_titles);
          memcpy(recv_titles, recv_buffer + sizeof(num_recv_titles),
            sizeof(char) * MAX_SIZE_TITLE * num_recv_titles);

          for (int i = 0; i < num_recv_titles; i++) {
            printf("Title: %s\n", &recv_titles[i * MAX_SIZE_TITLE]);
          }

          free(recv_titles);
        }
        break;
      case OP_GET_MOVIE_TITLE_OF_ID:
        {
          printf("Please, provide the movie id: ");
          int id = read_integer();

          send_buffer_with_option_and_id(socket_fd, (SA*) &serv_addr, sizeof(serv_addr), option, id);

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          char recv_title[150];
          memcpy(&recv_title, recv_buffer, sizeof(recv_title));

          printf("Title: %s\n", recv_title);
        }
        break;
      case OP_GET_MOVIE_OF_ID:
        {
          printf("Please, provide the movie id: ");
          int id = read_integer();

          send_buffer_with_option_and_id(socket_fd, (SA*) &serv_addr, sizeof(serv_addr), option, id);

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          Movie recv_movie;
          memcpy(&recv_movie, recv_buffer, sizeof(recv_movie));

          printf("Id: %d\n", recv_movie.id);
          printf("Title: %s\n", recv_movie.title);
          printf("Synopsis: %s\n", recv_movie.synopsis);
          printf("Genre: %s\n", recv_movie.genre);
        }
        break;
      case OP_GET_MOVIES:
        {
          send_buffer_with_option(socket_fd, (SA*) &serv_addr, sizeof(serv_addr), option);

          char recv_buffer[MAX_SIZE];
          if ((recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL)) < 0) {
            report_error();
          }
          int num_recv_movies;
          memcpy(&num_recv_movies, recv_buffer, sizeof(num_recv_movies));

          Movie *recv_movies = malloc(sizeof(Movie) * num_recv_movies);
          memcpy(recv_movies, recv_buffer + sizeof(num_recv_movies),
            sizeof(Movie) * num_recv_movies);

          for (int i = 0; i < num_recv_movies; i++) {
            printf("Id: %d \n", recv_movies[i].id);
            printf("Title: %s \n", recv_movies[i].title);
            printf("Synopsis: %s \n", recv_movies[i].synopsis);
            printf("Genre: %s\n\n", recv_movies[i].genre);
          }

          free(recv_movies);
        }
        break;
      default:
        report_error_msg("Invalid option chosen.\n");
    }
    option = read_integer();
  }

  close(socket_fd);

  return 0;
}

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
#include "database_queries.h"
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

void send_buffer_with_option(int socket_fd, int option) {
  char send_buffer[MAX_SIZE];
  memcpy(send_buffer, &option, sizeof(option));
  if ((write(socket_fd, send_buffer, sizeof(send_buffer))) < 0) {
    report_error();
  }
}

void send_buffer_with_option_and_id(int socket_fd, int option, int id) {
  char send_buffer[MAX_SIZE];
  memcpy(send_buffer, &option, sizeof(option));
  memcpy(send_buffer + sizeof(option), &id, sizeof(id));
  if ((write(socket_fd, send_buffer, sizeof(send_buffer))) < 0) {
    report_error();
  }
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
        write(socket_fd, send_buffer, sizeof(send_buffer));

        char recv_buffer[MAX_SIZE];
        int recv_id;
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        memcpy(&recv_id, recv_buffer, sizeof(recv_id));

        printf("Movie of id %d was successfully created.\n", recv_id);
      }
      break;
    case OP_REMOVE_MOVIE_ID:
      {
        printf("Please, provide the movie id: ");
        int id = read_integer();

        send_buffer_with_option_and_id(socket_fd, option, id);

        char recv_buffer[MAX_SIZE];
        int succeeded;
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
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
        send_buffer_with_option(socket_fd, option);

        char recv_buffer[MAX_SIZE];
        int num_recv_exhib_rooms;
        ExhibitionRoom *recv_exhib_rooms;
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        memcpy(&num_recv_exhib_rooms, recv_buffer, sizeof(num_recv_exhib_rooms));
        memcpy(recv_exhib_rooms, &recv_buffer[sizeof(num_recv_exhib_rooms)], sizeof(ExhibitionRoom) * num_recv_exhib_rooms);

        for (int i = 0; i < num_recv_exhib_rooms; i++) {
          printf("%d\n", recv_exhib_rooms[i].room_number);
          printf("%s\n", recv_exhib_rooms[i].movie_title);
        }
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
        write(socket_fd, send_buffer, sizeof(send_buffer));

        char recv_buffer[MAX_SIZE];
        int num_recv_titles;
        char *recv_titles[150];
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        memcpy(&num_recv_titles, recv_buffer, sizeof(num_recv_titles));
        for (int i = 0; i < num_recv_titles; i++) {
          memcpy(&recv_titles[i], recv_buffer, sizeof(recv_titles));
          printf("%s\n", &recv_titles[i]);
        }
      }
      break;
    case OP_GET_MOVIE_TITLE_OF_ID:
      {
        printf("Please, provide the movie id: ");
        int id = read_integer();

        send_buffer_with_option_and_id(socket_fd, option, id);

        char recv_buffer[MAX_SIZE];
        char recv_title[150];
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        memcpy(&recv_title, recv_buffer, sizeof(recv_title));

        printf("%s\n", recv_title);
      }
      break;
    case OP_GET_MOVIE_OF_ID:
      {
        printf("Please, provide the movie id: ");
        int id = read_integer();

        send_buffer_with_option_and_id(socket_fd, option, id);

        char recv_buffer[MAX_SIZE];
        Movie recv_movie;
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        memcpy(&recv_movie, recv_buffer, sizeof(recv_movie));

        printf("%d\n", recv_movie.id);
        printf("%s\n", recv_movie.title);
        printf("%s\n", recv_movie.synopsis);
        printf("%s\n", recv_movie.genre);
      }
      break;
    case OP_GET_MOVIES:
      {
        send_buffer_with_option(socket_fd, option);

        char recv_buffer[MAX_SIZE];
        int num_recv_movies;
        Movie *recv_movies;
        recv(socket_fd, recv_buffer, sizeof(recv_buffer), MSG_WAITALL);
        memcpy(&num_recv_movies, recv_buffer, sizeof(num_recv_movies));
        // TODO: memcpy array of movies
      }
      break;
    default:
      report_error_msg("Invalid option chosen.\n");
  }

  return 0;
}

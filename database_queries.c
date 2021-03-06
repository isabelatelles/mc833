/* ********************************************************************* *
 *
 * Database queries (implementation)
 * Description: responsible for the database queries that insert and
 * return information about the movies and exhibition rooms.
 *
 * Reference: http://zetcode.com/db/mysqlc/
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho - 187506
 *
 * ********************************************************************* */

#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "database_queries.h"
#include "client_server.h"
#include "errors.h"

MYSQL* connect_to_database() {
  MYSQL *con = mysql_init(NULL);

  if (con == NULL) {
    report_mysql_init_error();
  }

  if (mysql_real_connect(con, "database-2.c3eusmb3fdd7.us-east-1.rds.amazonaws.com",
    "admin", "thisissparta", "innodb", 0, NULL, 0) == NULL) {
    report_mysql_error(con);
  }

  return con;
}

void execute_query(MYSQL *con, const char query[]) {
  if (mysql_query(con, query)) {
    report_mysql_error(con);
  }
}

MYSQL_RES* execute_select_query(MYSQL *con, const char query[]) {
  execute_query(con, query);

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL) {
    report_mysql_error(con);
  }

  return result;
}

Movie* get_movies(int *size) {

  MYSQL *con = connect_to_database();

  MYSQL_RES *result = execute_select_query(con, "SELECT * FROM movie");

  int num_rows = mysql_num_rows(result);
  *size = num_rows;

  Movie *movies = malloc(sizeof(Movie) * num_rows);

  MYSQL_ROW row;

  int i = 0;
  while ((row = mysql_fetch_row(result))) {
    Movie movie;
    movie.id = atoi(row[0]);
    strcpy(movie.title, row[1]);
    strcpy(movie.synopsis, row[2]);
    strcpy(movie.genre, row[3]);
    movies[i] = movie;
    i++;
  }

  mysql_free_result(result);
  mysql_close(con);

  return movies;
}

Movie get_movie_of_id(int id) {

  MYSQL *con = connect_to_database();

  char query[50] = "SELECT * FROM movie WHERE idmovie = ";
  char idmovie[10];

  sprintf(idmovie, "%d", id);

  strcat(query, idmovie);

  MYSQL_RES *result = execute_select_query(con, query);

  Movie movie = {0};

  MYSQL_ROW row = mysql_fetch_row(result);

  if (row) {
    movie.id = atoi(row[0]);
    strcpy(movie.title, row[1]);
    strcpy(movie.synopsis, row[2]);
    strcpy(movie.genre, row[3]);
  } else {
    report_id_not_found_error(con, "No movie found with id", id);
  }

  mysql_free_result(result);
  mysql_close(con);

  return movie;
}

char* get_movie_title_of_id(int id) {
  MYSQL *con = connect_to_database();

  char query[50] = "SELECT title FROM movie WHERE idmovie = ";
  char idmovie[10];
  sprintf(idmovie, "%d", id);
  strcat(query, idmovie);

  MYSQL_RES *result = execute_select_query(con, query);

  MYSQL_ROW row = mysql_fetch_row(result);

  char *title = NULL;

  if (row) {
    title = malloc(sizeof(char) * strlen(row[0]) + 1);
    strcpy(title, row[0]);
  } else {
    report_id_not_found_error(con, "No movie found with id", id);
  }

  mysql_free_result(result);
  mysql_close(con);

  return title;
}

char* get_movie_titles_of_genre(char genre[], int *size) {
  MYSQL *con = connect_to_database();

  char query[50] = "SELECT title FROM movie WHERE genre = '";
  strcat(query, genre);
  strcat(query, "'");

  MYSQL_RES *result = execute_select_query(con, query);

  int num_rows = mysql_num_rows(result);
  *size = num_rows;

  char *titles = malloc(sizeof(char) * MAX_SIZE_TITLE * num_rows);

  MYSQL_ROW row;
  int i = 0;

  while ((row = mysql_fetch_row(result))) {
    strcpy(&titles[i * MAX_SIZE_TITLE], row[0]);
    i++;
  }

  mysql_free_result(result);
  mysql_close(con);

  return titles;
}

void create_room(int room_number, int movie_id) {
  MYSQL *con = connect_to_database();

  char query[800] = "INSERT INTO exhibition_room (room_number,fk_idmovie) VALUES (";
  char buffer[33];


  sprintf(buffer, "%d", room_number);
  strcat(query, buffer);
  strcat(query, ",");
  sprintf(buffer, "%d", movie_id);
  strcat(query, buffer);
  strcat(query, ")");
  strcat(query, " ON DUPLICATE KEY UPDATE fk_idmovie = ");
  sprintf(buffer, "%d", movie_id);
  strcat(query, buffer);
  execute_query(con, query);

  mysql_close(con);
}

int create_movie(Movie movie, int room_number) {
  MYSQL *con = connect_to_database();

  char query[800] = "INSERT INTO movie (title, synopsis, genre) VALUES('";
  strcat(query, movie.title);
  strcat(query, "','");
  strcat(query, movie.synopsis);
  strcat(query, "','");
  strcat(query, movie.genre);
  strcat(query, "')");
  execute_query(con, query);

  int id = mysql_insert_id(con);

  mysql_close(con);

  create_room(room_number, id);

  return id;
}

void remove_movie_id(int id){
  MYSQL *con = connect_to_database();

  char query[40] = "DELETE FROM movie WHERE idmovie = ";
  char idmovie[10];

  sprintf(idmovie, "%d", id);
  strcat(query, idmovie);

  execute_query(con, query);

  mysql_close(con);
}

ExhibitionRoom* get_exhibition_rooms(int *size){
  MYSQL *con = connect_to_database();
  char query[] = "SELECT room_number, title FROM exhibition_room INNER JOIN movie ON fk_idmovie = idmovie";

  MYSQL_RES *result = execute_select_query(con, query);

  int num_rows = mysql_num_rows(result);

  *size = num_rows;
  ExhibitionRoom* exhibition_rooms = malloc(sizeof(ExhibitionRoom) * num_rows);

  MYSQL_ROW row;
  int i = 0;

  while ((row = mysql_fetch_row(result))){
    exhibition_rooms[i].room_number = atoi(row[0]);
    strcpy(exhibition_rooms[i].movie_title, row[1]);
    i++;
  }

  mysql_free_result(result);
  mysql_close(con);

  return exhibition_rooms;
}
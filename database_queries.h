/* ********************************************************************* *
 *
 * Database queries
 * Description: responsible for the database queries that insert and return
 * information about the movies and exhibition rooms.
 *
 * Reference: http://zetcode.com/db/mysqlc/
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho - 187506
 *
 * ********************************************************************* */

#ifndef DATABASE_QUERIES_H
#define DATABASE_QUERIES_H

#include <mysql.h>

typedef struct MovieData {
  int id;
  char title[150];
  char synopsis[500];
  char genre[45];
} Movie;

typedef struct ExhibitionRoomData {
<<<<<<< HEAD
    int room_number;
    char movie_title[150];
=======
  int room_number;
  int id_movie;
>>>>>>> 4b966dd937e8f5f1df542a00f0c63b6568f86f71
} ExhibitionRoom;

MYSQL* connect_to_database();
void execute_query(MYSQL*, const char*);
MYSQL_RES* execute_select_query(MYSQL*, const char*);

Movie* get_movies();
Movie get_movie_of_id(int);
char* get_movie_title_of_id(int);
char** get_movie_titles_of_genre(char*);
int create_movie();
void remove_movie_id(int);
ExhibitionRoom* get_exhibition_rooms();

#endif

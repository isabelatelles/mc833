/* ********************************************************************* *
 *
 * Database queries
 * Description: responsible for the database queries that insert and return
 * information about the movies and exhibition rooms.
 * 
 * Reference: http://zetcode.com/db/mysqlc/
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho -
 *
 * ********************************************************************* */

#ifndef DATABASE_QUERIES_H
#define DATABASE_QUERIES_H

#include <my_global.h>
#include <mysql.h>

typedef struct MovieData {
    int id;
    char title[150];
    char synopsis[500];
    char genre[45];
} Movie;

typedef struct ExhibitionRoomData {
    int room_number;
    int id_movie;
} ExhibitionRoom;

MYSQL* connect_to_database();
void execute_query(MYSQL*, char*);
MYSQL_RES* execute_select_query(MYSQL*, char*);

Movie* get_movies();
Movie get_movie_of_id(int);
char* get_movie_title_of_id(int);
char** get_movie_titles_of_genre(char*);

#endif
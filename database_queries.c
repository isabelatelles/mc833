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

//#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "database_queries.h"
#include "errors.h"

MYSQL* connect_to_database() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "db-movies-mc833.cgmivothrwnj.us-east-1.rds.amazonaws.com",
        "admin", "thisissparta", "innodb", 0, NULL, 0) == NULL) {
        report_mysql_error(con);
    }

    return con;
}

void execute_query(MYSQL *con, char query[]) {
    if (mysql_query(con, query)) {
        report_mysql_error(con);
    }
}

MYSQL_RES* execute_select_query(MYSQL *con, char query[]) {
    execute_query(con, query);

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        report_mysql_error(con);
    }

    return result;
}

Movie* get_movies() {

    MYSQL *con = connect_to_database();

    MYSQL_RES *result = execute_select_query(con, "SELECT * FROM movie");

    Movie *movies = malloc(sizeof(Movie) * mysql_num_rows(result));

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
        Movie movie;
        movie.id = atoi(row[0]);
        strcpy(movie.title, row[1]);
        strcpy(movie.synopsis, row[2]);
        strcpy(movie.genre, row[3]);
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

    Movie movie;

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

    char* title;

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

char** get_movie_titles_of_genre(char genre[]) {
    MYSQL *con = connect_to_database();

    char query[50] = "SELECT title FROM movie WHERE genre = '";
    strcat(query, genre);
    strcat(query, "'");
    printf("%s\n", query);

    MYSQL_RES *result = execute_select_query(con, query);

    int num_rows = mysql_num_rows(result);

    char** titles = malloc(sizeof(char*) * num_rows);
    for (int i = 0; i < num_rows; i++) {
        titles[i] = malloc(sizeof(char) * 150);
    }

    MYSQL_ROW row;
    int i = 0;

    while ((row = mysql_fetch_row(result))) {
        strcpy(titles[i], row[0]);
        printf("%s\n", titles[i]);
        i++;
    }

    mysql_free_result(result);
    mysql_close(con);

    return titles;
}

int main(int argc, char **argv) {
    Movie *movies = get_movies();

    Movie movie = get_movie_of_id(1);

    printf("%d ", movie.id);
    printf("%s ", movie.title);
    printf("%s ", movie.synopsis);
    printf("%s ", movie.genre);
    printf("\n");
    // char *title = get_movie_title_of_id(1);
    // printf("%s\n", title);
    char **titles;
    titles = get_movie_titles_of_genre("acao");
     for (int i = 0; i < 2; i++) {
         printf("%s\n", titles[i]);
     }
    return 0;
}

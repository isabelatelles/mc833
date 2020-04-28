/* ********************************************************************* *
 *
 * Errors (implementation)
 * Description: responsible for reporting errors that occurred during
 * the execution.
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho - 187506
 *
 * ********************************************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <mysql.h>

#include "errors.h"

void report_error() {
  fprintf(stderr, "Error: %s\n", strerror(errno));
  exit(EXIT_FAILURE);
}

void report_error_msg(const char* msg) {
  fprintf(stderr, "Error: %s\n", msg);
  exit(EXIT_FAILURE);
}

void report_mysql_error(MYSQL *con) {
  fprintf(stderr, "Error: %s\n", mysql_error(con));
  mysql_close(con);
  exit(EXIT_FAILURE);
}

void report_mysql_init_error() {
  fprintf(stderr, "Error: mysql_init() failed\n");
  exit(EXIT_FAILURE);
}

void report_id_not_found_error(MYSQL *con, const char* message, int id) {
  fprintf(stderr, "Error: %s [%d].\n", message, id);
  mysql_close(con);
  exit(EXIT_FAILURE);
}

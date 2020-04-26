/* ********************************************************************* *
 *
 * Errors (implementation)
 * Description: responsible for reporting errors that occurred during
 * the execution.
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho -
 *
 * ********************************************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

#include "errors.h"

void report_mysql_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

void report_id_not_found_error(MYSQL *con, const char* message, int id) {
    fprintf(stderr, "%s [%d].\n", message, id);
    mysql_close(con);
    exit(1);
}
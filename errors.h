/* ********************************************************************* *
 *
 * Errors
 * Description: responsible for reporting errors that occurred during
 * the execution.
 *
 * Isabela Telles Furtado Doswaldo - 170012
 * Thamiris Florindo Coelho - 187506
 *
 * ********************************************************************* */

#ifndef ERRORS_H
#define ERRORS_H

#include <mysql.h>

void report_mysql_error(MYSQL*);
void report_mysql_init_error();
void report_id_not_found_error(MYSQL*, const char*, int);

#endif

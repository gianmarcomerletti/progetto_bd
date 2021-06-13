#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <mysql/mysql.h>

#include "defines.h"


typedef enum {
    ADMIN = 1,
    MANAGER,
    OPERATORE,
    FAILED_LOGIN
} role_t;

static char *opt_host_name = "127.0.0.1"; /* host (default=localhost) */
static char *opt_user_name = "login"; /* username (default=login name)*/
static char *opt_password = "login"; /* password (default=none) */
static unsigned int opt_port_num = 3306; /* port number (use built-in) */
static char *opt_socket_name = NULL; /* socket name (use built-in) */
static char *opt_db_name = "progetto"; /* database name (default=none) */
static unsigned int opt_flags = CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS; /* connection flags (none) */
static MYSQL *conn; /* pointer to connection handler */

char username[45];
char password[45];


static role_t attempt_login(MYSQL *conn, char *username, char *password) {
    MYSQL_STMT *login_procedure;

    MYSQL_BIND param[3];
    int role = 0;
    char statement[] = "call login(?,?,?);";

    login_procedure = mysql_stmt_init(conn);
    if (login_procedure == NULL) {
        fprintf (stderr, "%s\n", "Could not initialize statement handler");
        goto err2;
    }

    if (mysql_stmt_prepare(login_procedure, statement, strlen(statement)) != 0) {
        fprintf (stderr, "%s\n", "Could not prepare statement handler");
        goto err2;
    }

    // Prepara parametri
    memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_STRING; // IN
    param[0].buffer = username;
    param[0].buffer_length = strlen(username);

    param[1].buffer_type = MYSQL_TYPE_STRING; // IN
    param[1].buffer = password;
    param[1].buffer_length = strlen(password);

    param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
    param[2].buffer = &role;
    param[2].buffer_length = sizeof(role);

    if (mysql_stmt_bind_param(login_procedure, param) != 0) {
        fprintf(stderr, "Could not bind param for login procedure: %s", mysql_stmt_error(login_procedure));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(login_procedure) != 0) {
        fprintf(stderr, "Could not execute login procedure: %s", mysql_stmt_error(login_procedure));
        goto err;
    }

    // Prepara parametri di output
    memset(param, 0, sizeof(param));
    param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
    param[0].buffer = &role;
    param[0].buffer_length = sizeof(role);

    if(mysql_stmt_bind_result(login_procedure, param)) {
        fprintf(stderr, "Could not receive results of login procedure: %s", mysql_stmt_error(login_procedure));
        goto err;
    }

    // Ricevi parametri di output
    if(mysql_stmt_fetch(login_procedure)) {
        fprintf(stderr, "Could not buffer results of login procedure: %s", mysql_stmt_error(login_procedure));
        goto err;
    }

    mysql_stmt_close(login_procedure);
    return role;

    err:
    mysql_stmt_close(login_procedure);
    return FAILED_LOGIN;

    err2:
    return FAILED_LOGIN;
}

int main(void) {
    role_t role;

    conn = mysql_init (NULL);
    if (conn == NULL) {
        fprintf (stderr, "mysql_init() failed (probably out of memory)\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, opt_host_name, opt_user_name,
                           opt_password, opt_db_name, opt_port_num, opt_socket_name,
                           opt_flags) == NULL) {
        fprintf (stderr, "mysql_real_connect() failed. Error: %s\n", mysql_error(conn));
        mysql_close (conn);
        exit(EXIT_FAILURE);
    }

    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    role = attempt_login(conn, username, password);

    switch(role) {
        case MANAGER:
            printf("Login come MANAGER\n");
            run_as_manager(conn);
            break;

        case OPERATORE:
//			run_as_professor(conn);
            fprintf(stderr, "Login come OPERATORE\n");
            break;

        case ADMIN:
            printf("Login come ADMIN\n");
            run_as_admin(conn);
            break;

        case FAILED_LOGIN:
            fprintf(stderr, "Invalid credentials\n");
            exit(EXIT_FAILURE);

        default:
            abort();
    }

    printf("Bye!\n");

    mysql_close (conn);
    return 0;
}

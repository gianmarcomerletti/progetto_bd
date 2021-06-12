#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"

static void create_user(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    char r;
    char statement[] = "call crea_utente(?,?,?);";

    // Input for the registration routine
    char username[46];
    char password[46];
    char ruolo[46];

    // Get the required information
    printf("\nUsername: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    printf("Ruolo:\n");
    printf("\t1) Manager\n");
    printf("\t2) Operatore\n");
    printf("\t3) Admin\n");
    scanf(" %c", &r);

    // Convert role into enum value
    switch(r) {
        case '1':
            strcpy(ruolo, "manager");
            break;
        case '2':
            strcpy(ruolo, "operatore");
            break;
        case '3':
            strcpy(ruolo, "admin");
            break;
        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }

    prepared_stmt = mysql_stmt_init(conn);
    if (prepared_stmt == NULL) {
        fprintf (stderr, "%s\n", "Could not initialize statement handler");
        goto err;
    }

    if (mysql_stmt_prepare(prepared_stmt, statement, strlen(statement)) != 0) {
        fprintf (stderr, "%s\n", "Could not prepare statement handler");
        goto err;
    }

    // Prepare parameters
    memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[0].buffer = username;
    param[0].buffer_length = strlen(username);

    param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[1].buffer = password;
    param[1].buffer_length = strlen(password);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[2].buffer = ruolo;
    param[2].buffer_length = strlen(ruolo);

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        fprintf(stderr, "Could not bind param for procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        fprintf(stderr, "Could not execute procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    } else {
        printf("Utente aggiunto...\n");
        mysql_stmt_close(prepared_stmt);
        return;
    }

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

void run_as_admin(MYSQL *conn)
{
    char op;

    printf("Switching to admin role...\n");

    if(mysql_change_user(conn, "admin", "admin", "progetto")) {
        fprintf(stderr, "mysql_change_user() failed\n");
        exit(EXIT_FAILURE);
    }

    while(true) {
        printf("\033[2J\033[H");
        printf("*** MENU ***\n\n");
        printf("1) Crea nuovo utente\n");
        printf("2) Esci\n\n");

        printf("Seleziona un'opzione:\t");
        scanf(" %c", &op);

        switch(op) {
            case '1':
                create_user(conn);
                break;
            case '2':
                return;
            default:
                fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
                abort();
        }

        getchar();
    }
}
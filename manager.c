#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static int add_privato(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[7];
    char statement[] = "call aggiungi_privato(?,?,?,?,?,?,?);";

    // Input
    char nome[46];
    char residenza[46];
    char fatturazione[46];
    char contatto_pref[15];
    int contatto_pref_id;
    char contatto[46];
    char cf[17];
    int id_cliente;

    printf("\nNome completo: ");
    scanf(" %[^\n]", nome);
    printf("Indirizzo residenza: ");
    scanf(" %[^\n]", residenza);
    printf("Indirizzo fatturazione: ");
    scanf(" %[^\n]", fatturazione);
    printf("Metodo di contatto preferito [telefono, cellulare, email]: ");
    scanf(" %s", contatto_pref);
    printf("Contatto: ");
    scanf(" %s", contatto);
    printf("CF: ");
    scanf(" %s", cf);

    if (strcmp(contatto_pref, "telefono") == 0)
        contatto_pref_id = 1;
    else if (strcmp(contatto_pref, "cellulare") == 0)
        contatto_pref_id = 2;
    else if (strcmp(contatto_pref, "email") == 0)
        contatto_pref_id = 3;
    else
        contatto_pref_id = 0;


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
    param[0].buffer = nome;
    param[0].buffer_length = strlen(nome);

    param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[1].buffer = residenza;
    param[1].buffer_length = strlen(residenza);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[2].buffer = fatturazione;
    param[2].buffer_length = strlen(fatturazione);

    param[3].buffer_type = MYSQL_TYPE_LONG;
    param[3].buffer = &contatto_pref_id;
    param[3].buffer_length = sizeof (contatto_pref_id);

    param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[4].buffer = contatto;
    param[4].buffer_length = strlen(contatto);

    param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[5].buffer = cf;
    param[5].buffer_length = strlen(cf);

    param[6].buffer_type = MYSQL_TYPE_LONG; //OUT
    param[6].buffer = &id_cliente;
    param[6].buffer_length = sizeof(id_cliente);

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        fprintf(stderr, "Could not bind param for procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        fprintf(stderr, "Could not execute procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Prepara parametri di output
    memset(param, 0, sizeof(param));
    param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
    param[0].buffer = &id_cliente;
    param[0].buffer_length = sizeof(id_cliente);

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        fprintf(stderr, "Could not receive results of procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Ricevi parametri di output
    if(mysql_stmt_fetch(prepared_stmt)) {
        fprintf(stderr, "Could not buffer results of procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    mysql_stmt_close(prepared_stmt);
    return id_cliente;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void add_cliente(MYSQL *conn)
{
    int id;
    char r;

    printf("Tipologia cliente:\n");
    printf("\t1) Privato\n");
    printf("\t2) Rivendita\n");
    scanf(" %c", &r);

    // Convert role into enum value
    switch(r) {
        case '1':
            id = add_privato(conn);
            printf("Cliente inserito con ID %d...\n", id);
            break;
        case '2':

            break;
        case '3':
        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }
}

void run_as_manager(MYSQL *conn)
{
    char op;

    printf("Switching to manager role...\n");

    if(mysql_change_user(conn, "manager", "manager", "progetto")) {
        fprintf(stderr, "mysql_change_user() failed\n");
        exit(EXIT_FAILURE);
    }

    while(true) {
        printf("\033[2J\033[H");
        printf("*** MENU ***\n\n");
        printf("1) Aggiungi pianta\n");
        printf("2) Aggiungi cliente\n");
        printf("3) Aggiungi fornitura\n");
        printf("4) Aggiungi pianta\n");
        printf("5) Vedi inventario\n");
        printf("6) Vedi ordini\n");
        printf("7) Esci\n\n");

        printf("Seleziona un'opzione:\t");
        scanf(" %c", &op);

        switch(op) {
            case '1':
//                create_user(conn);
                break;
            case '2':
                add_cliente(conn);
                break;
            case '3':
//                create_user(conn);
                break;
            case '4':
//                create_user(conn);
                break;
            case '5':
//                create_user(conn);
                break;
            case '6':
//                create_user(conn);
                break;
            case '7':
                return;
            default:
                fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
                abort();
        }

        getchar();
    }
}


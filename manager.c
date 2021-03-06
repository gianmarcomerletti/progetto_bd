#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
        contatto_pref_id = 4;


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

static int add_rivendita(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[10];
    char statement[] = "call aggiungi_rivendita(?,?,?,?,?,?,?,?,?,?);";

    // Input
    char nome[46];
    char residenza[46];
    char fatturazione[46];
    char contatto_pref[15];
    int contatto_pref_id;
    char contatto[46];
    char p_iva[12];
    char referente[46];
    char contatto_pref_ref[15];
    int contatto_pref_ref_id;
    char contatto_ref[46];
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
    printf("Partita IVA: ");
    scanf(" %s", p_iva);
    printf("Nome completo del REFERENTE: ");
    scanf(" %s", referente);
    printf("Metodo di contatto preferito del REFERENTE [telefono, cellulare, email]: ");
    scanf(" %s", contatto_pref_ref);
    printf("Contatto del REFERENTE: ");
    scanf(" %s", contatto_ref);

    if (strcmp(contatto_pref, "telefono") == 0)
        contatto_pref_id = 1;
    else if (strcmp(contatto_pref, "cellulare") == 0)
        contatto_pref_id = 2;
    else if (strcmp(contatto_pref, "email") == 0)
        contatto_pref_id = 3;
    else
        contatto_pref_id = 4;

    if (strcmp(contatto_pref_ref, "telefono") == 0)
        contatto_pref_ref_id = 1;
    else if (strcmp(contatto_pref_ref, "cellulare") == 0)
        contatto_pref_ref_id = 2;
    else if (strcmp(contatto_pref_ref, "email") == 0)
        contatto_pref_ref_id = 3;
    else
        contatto_pref_ref_id = 4;


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
    param[5].buffer = p_iva;
    param[5].buffer_length = strlen(p_iva);

    param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[6].buffer = referente;
    param[6].buffer_length = strlen(referente);

    param[7].buffer_type = MYSQL_TYPE_LONG;
    param[7].buffer = &contatto_pref_ref_id;
    param[7].buffer_length = sizeof (contatto_pref_ref_id);

    param[8].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[8].buffer = contatto_ref;
    param[8].buffer_length = strlen(contatto_ref);

    param[9].buffer_type = MYSQL_TYPE_LONG; //OUT
    param[9].buffer = &id_cliente;
    param[9].buffer_length = sizeof(id_cliente);

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

static void add_contatto_privato(MYSQL *conn, int id)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    char statement[] = "call aggiungi_contatto_cliente(?,?,?);";

    // Input
    char contatto_tipo[46];
    char contatto[46];

    printf("Tipologia contatto [telefono, cellulare, email]: ");
    scanf(" %s", contatto_tipo);
    printf("Contatto: ");
    scanf(" %s", contatto);

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

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &id;
    param[0].buffer_length = sizeof(id);

    param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[1].buffer = &contatto;
    param[1].buffer_length = strlen(contatto);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[2].buffer = contatto_tipo;
    param[2].buffer_length = strlen(contatto_tipo);

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        fprintf(stderr, "Could not bind param for procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        fprintf(stderr, "Could not execute procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    mysql_stmt_close(prepared_stmt);
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void add_indirizzo_fornitore(MYSQL *conn, int id)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    char statement[] = "call aggiungi_contatto_cliente(?,?,?);";

    // Input
    char indirizzo[46];
    char alias[16];

    printf("Indirizzo: ");
    scanf(" %[^\n]", indirizzo);
    printf("Alias indirizzo: ");
    scanf(" %[^\n]", alias);

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

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &id;
    param[0].buffer_length = sizeof(id);

    param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[1].buffer = &indirizzo;
    param[1].buffer_length = strlen(indirizzo);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[2].buffer = alias;
    param[2].buffer_length = strlen(alias);

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        fprintf(stderr, "Could not bind param for procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        fprintf(stderr, "Could not execute procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    mysql_stmt_close(prepared_stmt);
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void add_cliente(MYSQL *conn)
{
    int id;
    char r,r2;

    printf("Tipologia cliente:\n");
    printf("\t1) Privato\n");
    printf("\t2) Rivendita\n");
    scanf(" %c", &r);

    switch(r) {
        case '1':
            id = add_privato(conn);
            printf("Cliente inserito con ID %d...\n", id);
            while (true) {
                printf("\nAggiungere un ulteriore contatto? [Y/N]\n");
                scanf(" %c", &r2);
                if (r2 == 'Y') {
                    add_contatto_privato(conn, id);
                    printf("Contatto inserito...\n");
                }
                else
                    break;
            }
            break;


        case '2':
            id = add_rivendita(conn);
            printf("Cliente inserito con ID %d...\n", id);
            while (true) {
                printf("\nAggiungere un ulteriore contatto? [Y/N]\n");
                scanf(" %c", &r2);
                if (r2 == 'Y') {
                    add_contatto_privato(conn, id);
                    printf("Contatto inserito...\n");
                }
                else
                    break;
            }
            break;

        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }
}

static void add_pianta(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[8];
    char statement[] = "call aggiungi_pianta(?,?,?,?,?,?,?,?);";

    // Input
    char nome_comune[46];
    char nome_lat[46];
    int disp;
    char fiorita, giardino, esotica;
    float prezzo;
    int id_pianta;

    printf("\nNome comune: ");
    scanf(" %[^\n]", nome_comune);
    printf("Nome latino: ");
    scanf(" %[^\n]", nome_lat);
    printf("Disponibilit??: ");
    scanf(" %d", &disp);
    printf("Pianta FIORITA? [0/1]");
    scanf(" %c", &fiorita);
    printf("Pianta DA GIARDINO? [0/1]");
    scanf(" %c", &giardino);
    printf("Pianta ESOTICA? [0/1]");
    scanf(" %c", &esotica);
    printf("Prezzo: ");
    scanf(" %f", &prezzo);

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
    param[0].buffer = nome_comune;
    param[0].buffer_length = strlen(nome_comune);

    param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[1].buffer = nome_lat;
    param[1].buffer_length = strlen(nome_lat);

    param[2].buffer_type = MYSQL_TYPE_LONG;
    param[2].buffer = &disp;
    param[2].buffer_length = sizeof(disp);

    param[3].buffer_type = MYSQL_TYPE_TINY;
    param[3].buffer = &fiorita;
    param[3].buffer_length = sizeof(fiorita);

    param[4].buffer_type = MYSQL_TYPE_TINY;
    param[4].buffer = &giardino;
    param[4].buffer_length = sizeof(giardino);

    param[5].buffer_type = MYSQL_TYPE_TINY;
    param[5].buffer = &esotica;
    param[5].buffer_length = sizeof(esotica);

    param[6].buffer_type = MYSQL_TYPE_FLOAT;
    param[6].buffer = &prezzo;
    param[6].buffer_length = sizeof(prezzo);

    param[7].buffer_type = MYSQL_TYPE_LONG; //OUT
    param[7].buffer = &id_pianta;
    param[7].buffer_length = sizeof(id_pianta);

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
    param[0].buffer = &id_pianta;
    param[0].buffer_length = sizeof(id_pianta);

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
    printf("Pianta inserita con ID %d...\n", id_pianta);
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void show_inventario(MYSQL *conn)
{
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    MYSQL_FIELD *fields;
    unsigned int i;

    // Esegui query
    if (mysql_query(conn, "select * from inventario;") != 0) {
        fprintf(stderr, "%s\n", "Could not execute query");
        goto err;
    }

    res_set = mysql_store_result(conn);
    if (res_set == NULL) {
        fprintf(stderr, "%s\n", "Could not store results");
        goto err;
    }

    printf("\n\n*** LISTA PIANTE MAGAZZINO ***\n");
    fields = mysql_fetch_field(res_set);
    mysql_field_seek (res_set, 0);
    for (i=0; i<mysql_num_fields(res_set); i++) {
        printf(" %-*s |", (int)fields[i].length, fields[i].name);
    }
    printf("\n");
    while ((row = mysql_fetch_row(res_set)) != NULL) {
        for (i=0; i<mysql_num_fields(res_set); i++) {
            printf(" %-*s |", (int)fields[i].length, row[i] != NULL ? row[i] : "NULL");
        }
        printf("\n");
    }
    return;

    err:
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void add_pianta_ordine(MYSQL *conn, int id)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    char statement[] = "call aggiungi_ordine(?,?,?);";

    // Input
    int cod_pianta;
    int quant;

    show_inventario(conn);
    printf("\nCod. pianta: ");
    scanf(" %d", &cod_pianta);
    printf("Quantita: ");
    scanf(" %d", &quant);

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

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &id;
    param[0].buffer_length = sizeof(id);

    param[1].buffer_type = MYSQL_TYPE_LONG;
    param[1].buffer = &cod_pianta;
    param[1].buffer_length = sizeof(cod_pianta);

    param[2].buffer_type = MYSQL_TYPE_LONG;
    param[2].buffer = &quant;
    param[2].buffer_length = sizeof(quant);

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        fprintf(stderr, "Could not bind param for procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        fprintf(stderr, "Could not execute procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    mysql_stmt_close(prepared_stmt);
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void create_ordine(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[5];
    char statement[] = "call crea_ordine(?,?,?,?,?);";

    // Input
    int cod_cliente;
    int cod_pianta;
    int quant;
    char ind_sped[46];
    int id_ordine;


    printf("\nCod. cliente: ");
    scanf(" %d", &cod_cliente);
    show_inventario(conn);
    printf("\nCod. pianta: ");
    scanf(" %d", &cod_pianta);
    printf("Quantita: ");
    scanf(" %d", &quant);
    printf("Indirizzo spedizione: ");
    scanf(" %[^\n]", ind_sped);

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

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &cod_cliente;
    param[0].buffer_length = sizeof(cod_cliente);

    param[1].buffer_type = MYSQL_TYPE_LONG;
    param[1].buffer = &cod_pianta;
    param[1].buffer_length = sizeof(cod_pianta);

    param[2].buffer_type = MYSQL_TYPE_LONG;
    param[2].buffer = &quant;
    param[2].buffer_length = sizeof(quant);

    param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[3].buffer = ind_sped;
    param[3].buffer_length = strlen(ind_sped);

    param[4].buffer_type = MYSQL_TYPE_LONG; //OUT
    param[4].buffer = &id_ordine;
    param[4].buffer_length = sizeof(id_ordine);

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
    param[0].buffer = &id_ordine;
    param[0].buffer_length = sizeof(id_ordine);

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

    char r;
    while (true) {
        printf("\nAggiungere pianta all'ordine? [Y/N]\n");
        scanf(" %c", &r);
        if (r == 'Y') {
            add_pianta_ordine(conn, id_ordine);
            printf("Pianta inserita...\n");
        }
        else
            break;
    }


    printf("Ordine creato con ID %d...\n", id_ordine);
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void add_fornitore(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[5];
    char statement[] = "call aggiungi_fornitore(?,?,?,?,?);";
    char r;

    // Input
    char nome[46];
    char cf[17];
    char indirizzo[46];
    char alias[16];
    int id_fornitore;

    printf("\nNome completo: ");
    scanf(" %[^\n]", nome);
    printf("CF: ");
    scanf(" %s", cf);
    printf("Indirizzo: ");
    scanf(" %[^\n]", indirizzo);
    printf("Alias indirizzo: ");
    scanf(" %[^\n]", alias);

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
    param[1].buffer = cf;
    param[1].buffer_length = strlen(cf);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[2].buffer = indirizzo;
    param[2].buffer_length = strlen(indirizzo);

    param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
    param[3].buffer = alias;
    param[3].buffer_length = strlen(alias);

    param[4].buffer_type = MYSQL_TYPE_LONG; //OUT
    param[4].buffer = &id_fornitore;
    param[4].buffer_length = sizeof(id_fornitore);

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
    param[0].buffer = &id_fornitore;
    param[0].buffer_length = sizeof(id_fornitore);

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
    printf("Fornitore inserito con ID %d...\n", id_fornitore);
    while (true) {
        printf("\nAggiungere un ulteriore indirizzo? [Y/N]\n");
        scanf(" %c", &r);
        if (r == 'Y') {
            add_indirizzo_fornitore(conn, id_fornitore);
            printf("Indirizzo inserito...\n");
        }
        else
            break;
    }
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

static void ordini_cliente(MYSQL *conn)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[4];
    char statement[] = "call report_ordini(?);";

    MYSQL_RES *rs_metadata;
    MYSQL_FIELD *fields;
    unsigned int i;
    int num_fields;

    // Input
    int cod_cliente;

    printf("Cod. cliente: ");
    scanf(" %d", &cod_cliente);

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

    param[0].buffer_type = MYSQL_TYPE_LONG;
    param[0].buffer = &cod_cliente;
    param[0].buffer_length = sizeof(cod_cliente);

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        fprintf(stderr, "Could not bind param for procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    // Esegui procedura
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        fprintf(stderr, "Could not execute procedure: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }
    mysql_stmt_store_result(prepared_stmt);
    num_fields = mysql_stmt_field_count(prepared_stmt);
    rs_metadata = mysql_stmt_result_metadata(prepared_stmt);
    if (rs_metadata == NULL) {
        fprintf(stderr, "%s\n", "Could not retrive result metadata");
        goto err;
    }

    fields = mysql_fetch_fields(rs_metadata);

    // Prepara parametri di output
    memset(param, 0, sizeof (MYSQL_BIND) * num_fields);

    printf("\n\n*** LISTA ORDINI per il CLIENTE cod:%d ***\n", cod_cliente);
    for (i=0; i<num_fields; i++) {
        switch(fields[i].type) {
            case MYSQL_TYPE_DATETIME:
                param[i].buffer = malloc(sizeof(MYSQL_TIME) + 1);
                param[i].buffer_length = sizeof(MYSQL_TIME) + 1;
                break;
            case MYSQL_TYPE_FLOAT:
                param[i].buffer = malloc(sizeof(float) + 1);
                param[i].buffer_length = sizeof(float) + 1;
                break;
            case MYSQL_TYPE_LONG:
                param[i].buffer = malloc(sizeof(int) + 1);
                param[i].buffer_length = sizeof(int) + 1;
                break;
            default:
                param[i].buffer = malloc(fields[i].max_length + 1);
                param[i].buffer_length = fields[i].max_length + 1;
                break;
        }
        param[i].buffer_type = fields[i].type;

        if(param[i].buffer == NULL) {
            printf("Cannot allocate output buffers\n");
            goto err;
        }

        printf(" %-*s |", (int)fields[i].length, fields[i].name);
    }
    printf("\n");
    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        fprintf(stderr, "Could not bind output param: %s", mysql_stmt_error(prepared_stmt));
        goto err;
    }

    int status;
    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);
        if (status == 1 || status == MYSQL_NO_DATA)
            break;

        for (i=0; i<num_fields; i++) {
            switch (param[i].buffer_type) {
                case MYSQL_TYPE_VAR_STRING:
                case MYSQL_TYPE_DATETIME:
                    printf(" %-*s |", (int)fields[i].length, (char *)param[i].buffer);
                    break;
                case MYSQL_TYPE_STRING:
                    printf(" %-*s |", (int)fields[i].length, (char *)param[i].buffer);
                    break;
                case MYSQL_TYPE_FLOAT:
                    printf(" %.02f |", *(float *)param[i].buffer);
                    break;
                case MYSQL_TYPE_LONG:
                    printf(" %-*d |", (int)fields[i].length, *(int *)param[i].buffer);
                    break;
                default:
                    printf("ERROR: Unhandled type (%d)\n", param[i].buffer_type);
                    abort();
            }
        }
    }

    mysql_free_result(rs_metadata); /* free metadata */;
    mysql_stmt_close(prepared_stmt);
    return;

    err:
    mysql_stmt_close(prepared_stmt);
    mysql_close(conn);
    exit(EXIT_FAILURE);
}

void run_as_manager(MYSQL *conn)
{
    char op;

    printf("Switching to manager role...\n");

    if(mysql_change_user(conn, "manager", "manager", "progetto")) {
        fprintf(stderr, "mysql_change_user() failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\033[2J\033[H");
    while(true) {
        printf("\n\n*** MENU ***\n\n");
        printf("1) Aggiungi pianta\n");
        printf("2) Aggiungi cliente\n");
        printf("3) Aggiungi fornitore\n");
        printf("4) Crea ordine\n");
        printf("5) Vedi inventario\n");
        printf("6) Vedi ordine cliente\n");
        printf("7) Esci\n\n");

        printf("Seleziona un'opzione:\t");
        scanf(" %c", &op);
        printf("\033[2J\033[H");

        switch(op) {
            case '1':
                add_pianta(conn);
                break;
            case '2':
                add_cliente(conn);
                break;
            case '3':
                add_fornitore(conn);
                break;
            case '4':
                create_ordine(conn);
                break;
            case '5':
                show_inventario(conn);
                break;
            case '6':
                ordini_cliente(conn);
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


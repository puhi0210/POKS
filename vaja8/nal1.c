#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main() {
    MYSQL *conn; // Povezava
    MYSQL_RES *res; // rezultat
    MYSQL_ROW row; // vrstuca
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "";
    const char *database = "test";
    // Vzpostavimo povezave z podatkovno baze
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }
    if (mysql_real_connect(conn, server, user, password, database, 0,
        NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    // Izvedemo SQL Poizvedbo
    if (mysql_query(conn, "SELECT * FROM Ports")) {
        fprintf(stderr, "SELECT query failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    // Spravimo rezulat poizvedbe v res
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    // število polj v poizvedbi
    int num_fields = mysql_num_fields(res);
    // izberemo vsako polje v poizvedbi in ga izpišemo
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    // Sprostimo rezultat in zapremo povezavo
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
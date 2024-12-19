#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>

int main() {
    // Inicializacija mongo C driverja
    mongoc_init();

    // Podatki za mongo DB
    const char *uri_string = "mongodb://localhost:27017";
    mongoc_client_t *client;
    mongoc_database_t *database;
    bson_error_t error;

    // Nova instanca mongo klienta
    client = mongoc_client_new(uri_string);
    if (!client) {
        fprintf(stderr, "Failed to connect to MongoDB\n");
        return EXIT_FAILURE;
    }

    // Povežemo se na DB
    database = mongoc_client_get_database(client, "vaje");

    // Pridobimo vse podatke iz DB
    char **collections =
        mongoc_database_get_collection_names_with_opts(database, NULL, &error);
    if (!collections) {
        fprintf(stderr, "Error fetching collections: %s\n", error.message);
        mongoc_database_destroy(database);
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }

    // Pregledamo vse zapise v tabeli
    printf("Displaying all data in the database:\n");
    for (char **coll_name = collections; *coll_name != NULL; coll_name++) {
        printf("\nCollection: %s\n", *coll_name);

        mongoc_collection_t *collection =
            mongoc_database_get_collection(database, *coll_name);

        // Pridobimo vse dokumente
        mongoc_cursor_t *cursor =
            mongoc_collection_find_with_opts(collection, bson_new(), NULL, NULL);
        const bson_t *doc;

        // Loop čez najdene dokumente
        while (mongoc_cursor_next(cursor, &doc)) {
            char *json = bson_as_canonical_extended_json(doc, NULL);
            printf("%s\n", json);
            bson_free(json);
        }

        if (mongoc_cursor_error(cursor, &error)) {
            fprintf(stderr, "Cursor error in collection %s: %s\n", *coll_name, error.message);
        }

        // Počistimo za sabo
        mongoc_cursor_destroy(cursor);
        mongoc_collection_destroy(collection);
    }

    bson_strfreev(collections);

    // Počistimo povezavo
    mongoc_database_destroy(database);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return EXIT_SUCCESS;
}

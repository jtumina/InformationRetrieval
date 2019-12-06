#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

#include "hashtable.h"
#include "infoRetrieval.h"

int main (int argc, char** argv) {

    if (argc != 3) {
        exit (0);
    }

    glob_t result;

    if (glob("./p5docs/*.txt", 0, 0, &result) != 0) {
        printf("Error: Problem with glob\n");
        exit (0);
    }

    struct hashtable* ht = ht_create (atoi (argv[1]));

    ht->docIDs = result.gl_pathv;
    ht->num_docs = result.gl_pathc;

    int* query_len = (int*) malloc (sizeof (int));

    // Check for allocation errors
    if (query_len == NULL) {
        printf("Error: unable to allocate memory for query_len\n");
        exit (0);
    }

	// Split the str provided into an array of strings
    char** search_query = read_query (argv[2], query_len);

	// Train the hashtable, then rank the files based on the query
    train (ht);
    rank (ht, search_query, *query_len);

	// Deallocate memory
	ht_destroy (ht);
	free (search_query);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "infoRetrieval.h"

int main (int argv, char** argc) {
    char** docs = (char**) malloc (3 * sizeof (char*));
    docs[0] = "D1.txt";
    docs[1] = "D2.txt";
    docs[2] = "D3.txt";

    struct hashtable* ht = ht_create (argv);

    ht->docIDs = docs;
    ht->num_docs = 3;

    int* query_len = (int*) malloc (sizeof (int));

    char** search_query = read_query (argc[0], query_len);
    train (ht);
    rank (ht, search_query, *query_len);
}

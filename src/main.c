#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "infoRetrieval.h"

int main (int argc, char** argv) {

    if (argc != 3) {
        exit (0);
    }

    char** docs = (char**) malloc (3 * sizeof (char*));
    /*docs[0] = "../p5docs/D1.txt";
    docs[1] = "../p5docs/D2.txt";
    docs[2] = "../p5docs/D3.txt";*/
	docs[0] = "D4.txt";

    struct hashtable* ht = ht_create (5/*(int) argv[1]*/);

    ht->docIDs = docs;
    ht->num_docs = 1;

    int* query_len = (int*) malloc (sizeof (int));

    char** search_query = read_query (argv[2], query_len);
    train (ht);
    rank (ht, search_query, *query_len);
}

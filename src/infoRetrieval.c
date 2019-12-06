/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Handles processes of populating the hashtable, removing stop words, reading the
* search-query, and ranking the documents by relevancy.
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "hashtable.h"
#include "infoRetrieval.h"
#include "sort.h"

/**
 * Computes the idf of the given word.
 * idf = log (N / df) ,
 * where N = number of documents in hashtable, and df = document frequency
 * @param  wn pointer to a wordNode
 * @return the computed idf
 */
double get_idf (struct hashtable* ht, struct wordNode* wordPtr) {
    double N = ht->num_docs;
    double df = 0;

    // Check to make sure word exists before accessing
    if (wordPtr != NULL) {
        df = wordPtr->df;
    } else { // Add 1 to df to prevent divide by 0
        df++;
    }

    return log10 (N / df);
}

/**
 * Searches the given hashtable for this word and doc_id pair.
 * @param  word    char* to the word to search for
 * @param  doc_id  char* to the document this word belongs in
 * @return the tf of this word in this doc_id
 */
int get_tf (struct wordNode* wordPtr, char* doc_id) {
    // If wordPtr == NULL, word doesn't exists in hashtable, tf = 0
    if (wordPtr == NULL) {
        return 0;
    }

    // Set pointer to head of doc list
    struct docNode* docPtr = wordPtr->docHead;

    // Loop through doc list searching for the document
    while (docPtr != NULL && strcmp (docPtr->doc_id, doc_id) != 0) {
        docPtr = docPtr->next;
    }

    // If we reached NULL, this word does not belong in the particular document
    if (docPtr == NULL) {
        return 0;
    }

    // Else, we found the word
    return docPtr->tf;
}

/**
 * Removes all words in hash table whose idf = 0.
 * @param ht pointer to the hashtable
 */
void stop_words (struct hashtable* ht) {
    // Loop through buckets in hashtable
    for (int i = 0; i < ht->num_buckets; i++) {

        // Get head of word list at this bucket
        struct wordNode* wordPtr = ht->map[i];
        // Temp pointer to point to node before wordPtr to stitch list together
        struct wordNode* lastPtr = ht->map[i];

        // Loop through word list
        while (wordPtr != NULL) {
            // If the idf of this word == 0, it is a stop word and we need to remove it
            if (get_idf (ht, wordPtr) == 0) {
                // Free the fields of this wordPtr
                free (wordPtr->word);
                destroy_docList (wordPtr->docHead);

                // Stich the node before this to the one after this
                lastPtr->next = wordPtr->next;

                free (wordPtr);
            }
            lastPtr = wordPtr;
            wordPtr = wordPtr->next;
        }
    }
}

/**
 * Takes a set of documents and populates the hashtable.
 * @param  ht    pointer to the hashtable
 */
void train (struct hashtable* ht) {
    // Pointer to file
    FILE* f;

    // Loop through the set of documents
    for (int i = 0; i < ht->num_docs; i++) {
        f = fopen (ht->docIDs[i], "r");

        // Check for NULL files
        if (f == NULL) {
            printf("Error in opening %s: ", ht->docIDs[i]);
            printf("%s\n", strerror(errno));
            exit(0);
        }

        char c;
        char* word = (char*) malloc (21 * sizeof (char));

        // Check for allocation errors
        if (word == NULL) {
            printf("Error: unable to allocate memory for word\n");
            exit (0);
        }

        int j = 0;

        // Read each char from doc until EOF
        while ((c = getc (f)) != EOF) {
            // If char is a space or \n, this is the end of a word
            if (c == ' ' || c == '\n') {
                // Set the last char of the word as the string terminator
                word[j] = '\0';
                ht_insert (ht, word, ht->docIDs[i]);
                word = (char*) malloc (21 * sizeof (char));

                // Check for allocation errors
                if (word == NULL) {
                    printf("Error: unable to allocate memory for word\n");
                    exit (0);
                }

                j = 0;
            } else {
                word[j] = c;
                j++;
            }
        }
        // If j did not get reset to 0, then add this word to hashtable
        if (j != 0) {
            ht_insert (ht, word, ht->docIDs[i]);
        }
        fclose (f);
    }
    // Remove stop words from hashtable as last step of training process
    stop_words (ht);
}

/**
 * Reads in search query from user at console.
 * @param str        the string that needs to be split into separate words
 * @paran query_len  parameter will hold the length of the search_query
 * @return array of strings where each string is each search term
 */
char** read_query (char* str, int* query_len) {
    if (str == NULL) {
        printf("Error: search query cannot be empty.\n");
        exit (0);
    }

    char** search_query = (char**) malloc (2 * sizeof (char*));

    if (search_query == NULL) {
        printf("Memory Error: Not enough space for search_query.\n");
        exit (0);
    }

    search_query[0] = strtok (str, " ");
    int i = 1;

    // Loop through str and extract each word
    // Assume no word exceeds 20 characters
    while ((search_query[i] = strtok (NULL, " ")) != NULL) {
        search_query = (char**) realloc (search_query, (i+2) * sizeof (char*));

        if (search_query == NULL) {
            printf("Memory Error: Not enough space for search_query.\n");
            exit (0);
        }

        i++;
    }
    *query_len = i;
    return search_query;
}

/**
 * Computes the tf-idf score for this document and givens set of search terms
 * @param  ht           pointer to the hashtable we are working with
 * @param  search_query string array of search terms
 * @param  query_len    length of the search query
 * @param  doc_id       char* to the doc_id we are computing tf-idf for
 * @return              the relevancy score of this doc_id against the given search_query
 */
double compute_tf_idf (struct hashtable* ht, char** search_query, int query_len, char* doc_id) {
    // Relevancy score for this doc
    double r = 0;

    // Loop through words in search_query
    for (int j = 0; j < query_len; j++) {
        // Find the word in the hashtable
        struct wordNode* wordPtr = get_word (ht, search_query[j]);

        // Get the tf and idf for this word, doc_id pair
        int tf = get_tf (wordPtr, doc_id);
        double idf = get_idf (ht, wordPtr);

        // Compute tf*idf and add it to the score for this doc
        r += (tf * idf);
    }
    return r;
}

/**
 * Print the contents of the most relvant document to console and list all files and
 * their scores in order in search_scores.txt
 * @param  ht           pointer to the hashtable we are working with
 * @param scores array of relevancy_scores
 */
void output_results (struct hashtable* ht, struct relevancy_score* scores) {
    // Open the file with the highest relevancy score
    FILE* f = fopen (scores[0].doc_id, "r");

    // Check for NULL to prevent crash
    if (f == NULL) {
        printf("Error: %s is NULL.\n", scores[0].doc_id);
        return;
    }

    // Print the cotents of the file, char by char
    char c;
    while ((c = getc (f)) != EOF) {
        printf("%c", c);
    }

    fclose (f);

    // Create search_scores.txt
    f = fopen ("search_scores.txt", "w");

    // Print each file and its score
    for (int i = 0; i < ht->num_docs; i++) {
        fprintf (f, "%s:%f\n", scores[i].doc_id, scores[i].score);
    }

    fclose (f);
}

/**
 * Ranks the documents in order of their tf-idf scores and outputs results.
 * @param  ht            pointer to the hashtable
 * @param  search_query  string array of search terms
 * @param  query_len     length of the search query
 */
void rank (struct hashtable* ht, char** search_query, int query_len) {
    // Array of relevancy_score structs
    struct relevancy_score scores[ht->num_docs];

    // Loop through documents and compute tf-idf for each one,
    for (int i = 0; i < ht->num_docs; i++) {
        //scores[i] = (struct relevancy_score) malloc (sizeof (struct relevancy_score));
        scores[i].doc_id = ht->docIDs[i];
        scores[i].score = compute_tf_idf (ht, search_query, query_len, ht->docIDs[i]);
    }

    // Sort the doc_ids according to their tf-idf scores
    sort (ht, scores);

    output_results (ht, scores);
}

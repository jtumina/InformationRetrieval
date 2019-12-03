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
double get_idf (struct hashtable ht, struct wordNode* wordPtr) {
    double N = ht->num_docs;
    double df = 0;

    // Check to make sure word exists before accessing
    if (wordPtr != NULL) {
        df = wordPtr->df;
    } else { // Add 1 to df to prevent divide by 0
        df++;
    }

    return log (N / df);
}

/**
 * Searches the given hashtable for this word and doc_id pair.
 * @param  ht      pointer to the hashtable to search in
 * @param  word    char* to the word to search for
 * @param  doc_id  char* to the document this word belongs in
 * @return the tf of this word in this doc_id
 */
int get_tf (struct hashtable* ht, struct wordNode* wordPtr, char* doc_id) {
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
        return NULL;
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
            if (compute_idf (ht, wordPtr) == 0) {
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
 * @return 1 if succesful and 0 if not
 */
int train (struct hashtable* ht) {
    // Pointer to file
    FILE* f;
    char* buf[22];

    // Loop through the set of documents
    for (int i = 0; i < ht->num_docs; i++) {
        f = fopen (ht->docIDs[i], "r");

        // Check for NULL files
        if (f == NULL) {
            printf("Error: %s is NULL.\n", ht->docIDs[i]);
            return 0;
        }

        // Loop through file, adding each word to the hashtable
        while (fscanf (f, "%s", buf) != NULL) {
            ht_insert (ht, buf, ht->docIDs[i]);
        }
    }
    return 1;
}

/**
 * Reads in search query from user at console.
 * @return array of strings where each string is each search term
 */
char** read_query () {
    char** words;
    return words;
}

/**
 * Computes the tf-idf score for this document and givens set of search terms
 * @param  ht           pointer to the hashtable we are working with
 * @param  search_query string array of search terms
 * @param  doc_id       char* to the doc_id we are computing tf-idf for
 * @return              the relevancy score of this doc_id against the given search_query
 */
double compute_tf_idf (struct hashtable* ht, char** search_query, char* doc_id) {
    // Relevancy score for this doc
    double r = 0;

    // Compute the length of the search_query
    int query_length = (int) (sizeof (search_query)/ sizeof (char*));

    // Loop through words in search_query
    for (int j = 0; j < query_length; j++) {
        // Find the word in the hashtable
        struct wordNode* wordPtr = get_word (ht, search_query[j]);

        // Get the tf and idf for this word, doc_id pair
        int tf = get_tf (ht, wordPtr, doc_id);
        int idf = get_idf (ht, wordPtr, doc_id);

        // Compute tf*idf and add it to the score for this doc
        r += (tf * idf);
    }
    return r;
}

/**
 * Print the contents of the most relvant document to console and list all files and their scores
 * in order in search_scores.txt
 * @param scores array of relevancy_scores
 */
void output_results (struct relevancy_score** scores) {
    // Open the file with the highest relevancy score
    FILE* f = fopen (scores[0]->doc_id, "r");

    // Check for NULL to prevent crash
    if (f == NULL) {
        printf("Error: %s is NULL.\n", scores[0]->doc_id);
        return;
    }

    // Print the cotents of the file, char by char
    char c;
    while (c = getc (f) != EOF) {
        printf("%c\n", c);
    }

    fclose (f);

    // Create search_scores.txt
    f = fopen ("search_scores.txt", "w");

    // Get length of scores
    int len = (int) (sizeof (scores) / sizeof (scores[0]));

    // Print each file and its score
    for (int i = 0; i < len; i++) {
        fputs ("%s:%f\n", scores[i]->doc_id, scores[i]->score);
    }

    fclose (f);
}

/**
 * Ranks the documents in order of their tf-idf scores and outputs results.
 * @param  ht           pointer to the hashtable
 * @param search_query  string array of search terms
 */
void rank (struct hashtable* ht, char** search_query) {
    // Array of relevancy_score structs
    struct relevancy_score** scores
        = (struct relevancy_score**) malloc (ht->num_docs * sizeof (struct relevancy_score*));

    // Loop through documents and compute tf-idf for each one,
    for (int i = 0; i < ht->num_docs; i++) {
        scores[i] = (struct relevancy_score*) malloc (sizeof (struct relevancy_score));
        scores[i]->doc_id = ht->docIDs[i];
        scores[i]->score = compute_tf_idf (ht, search_query, ht->docIDs[i]);
    }
    // Sort the doc_ids according to their tf-idf scores
    sort (scores);

    output_results (scores);
}

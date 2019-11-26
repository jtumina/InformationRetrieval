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
#include "infoRetrieval"

/**
 * Computes the idf of the given word.
 * idf = log (N / df) ,
 * where N = number of documents in hashtable, and df = document frequency
 * @param  wn pointer to a wordNode
 * @return the computed idf
 */
double get_idf (struct hashtable ht, struct wordNode* wordPtr) {
    double N = ht->num_files;
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
int get_tf (struct hashtable* ht, char* word, char* doc_id) {
    // Get wordNode this word resides in
    struct wordNode* wordPtr = get_word (ht, word);

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
 * @param  docs  the set of documents to be added
 * @return 1 if succesful and 0 if not
 */
int train (struct hashtable* ht, char** docs) {

}

/**
 * Reads in search query from user at console.
 * @return array of strings where each string is each search term
 */
char** read_query () {

}

/**
 * Computes tf-idf score for each document.
 * @param  ht           pointer to the hashtable
 * @param search_query  string array of search terms
 * @return doc_id of the document with highest tf-idf score
 *         or NULL if search query does not appear in any document.
 */
char* rank (struct hashtable* ht, char** search_query) {
    // Array of relevancy scores for each doc,
    // where r[i] is the relevancy score of docIDS[i]
    double r[ht->num_docs];

    int query_length = (int) (sizeof (search_query)/ sizeof (char*));

    // Loop through documents
    for (int i = 0; i < ht->num_docs; i++) {
        // Loop through words in search_query
        for (int j = 0; j < query_length; j++) {
            // Find the word in the hashtable
            struct wordNode* wordPtr = get_word (ht, search_query[j]);

            // Get the tf and idf for this word, doc_id pair
            int tf = get_tf (ht, wordPtr);
            int idf = get_idf (ht, wordPtr, doc_id);

            // Compute tf*idf and add it to the score for this doc
            r[i] += (tf * idf);
        }
    }
}

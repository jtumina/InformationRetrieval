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
double compute_idf (struct hashtable ht, struct wordNode* wordPtr) {
    double N = ht->num_files;
    double df = wordPtr->df;

    // If df == 0, set it to 1 to prevent divide by 0 errors
    df = (df == 0) ? 1 : df;

    return log (N / df);
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
                // Free the char* to the word
                free (wordPtr->word);
                // Free the doc list for this word
                destroy_docList (wordPtr->docHead);
                // Stich the node before this to the one after this
                lastPtr->next = wordPtr->next;
                // Now free the wordNode itself
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
 * @param  ht    pointer to the hashtable
 * @param  docs  list of all documents in hashtable
 * @return doc_id of the document with highest tf-idf score
 *         or NULL if search query does not appear in any document.
 */
char* rank (struct hashtable* ht) {

}

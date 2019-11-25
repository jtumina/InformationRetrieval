/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Handles processes of populating the hashtable, removing stop words, reading the
* search-query, and ranking the documents by relevancy.
***************************************************************************************/

#ifndef infoRetrieval_H
#define infoRetrieval_H

/**
 * Computes the idf of the given word.
 * idf = log (N / df) ,
 * where N = number of documents in hashtable, and df = document frequency
 * @param  wn pointer to a wordNode
 * @return the computed idf
 */
int compute_idf (struct hashtable ht, struct wordNode* wordPtr);
/**
 * Removes all words in hash table whose idf = 0.
 * @param ht pointer to the hashtable
 */
void stop_words (struct hashtable* ht);
/**
 * Takes a set of documents and populates the hashtable.
 * @param  ht    pointer to the hashtable
 * @param  docs  the set of documents to be added
 * @return 1 if succesful and 0 if not
 */
int train (struct hashtable* ht, char** docs);
/**
 * Reads in search query from user at console.
 * @return array of strings where each string is each search term
 */
char** read_query ();
/**
 * Computes tf-idf score for each document.
 * @param  ht    pointer to the hashtable
 * @param  docs  list of all documents in hashtable
 * @return doc_id of the document with highest tf-idf score
 *         or NULL if search query does not appear in any document.
 */
char* rank (struct hashtable* ht);

#endif

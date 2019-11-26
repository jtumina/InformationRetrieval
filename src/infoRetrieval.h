/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Handles processes of populating the hashtable, removing stop words, reading the
* search-query, and ranking the documents by relevancy.
***************************************************************************************/

#ifndef infoRetrieval_H
#define infoRetrieval_H

struct relevancy_score {
    char* doc_id;
    double score;
};

/**
 * Computes the idf of the given word.
 * idf = log (N / df) ,
 * where N = number of documents in hashtable, and df = document frequency
 * @param  wn pointer to a wordNode
 * @return the computed idf
 */
int get_idf (struct hashtable ht, struct wordNode* wordPtr);
/**
 * Searches the given hashtable for this word and doc_id pair.
 * @param  ht      pointer to the hashtable to search in
 * @param  word    char* to the word to search for
 * @param  doc_id  char* to the document this word belongs in
 * @return the tf of this word in this doc_id
 */
int get_tf (struct hashtable* ht, char* word, char* doc_id);
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
 * Computes the tf-idf score for this document and givens set of search terms
 * @param  ht           pointer to the hashtable we are working with
 * @param  search_query string array of search terms
 * @param  doc_id       char* to the doc_id we are computing tf-idf for
 * @return              the relevancy score of this doc_id against the given search_query
 */
double compute_tf_idf (struct hashtable* ht, char** search_query, char* doc_id);
/**
 * Computes tf-idf score for each document.
 * @param  ht           pointer to the hashtable
 * @param search_query  string array of search terms
 * @return doc_id of the document with highest tf-idf score
 *         or NULL if search query does not appear in any document.
 */
char* rank (struct hashtable* ht, char** search_query);

#endif

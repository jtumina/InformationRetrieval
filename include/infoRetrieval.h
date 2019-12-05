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
double get_idf (struct hashtable* ht, struct wordNode* wordPtr);

/**
 * Searches the given hashtable for this word and doc_id pair.
 * @param  ht      pointer to the hashtable to search in
 * @param  word    char* to the word to search for
 * @param  doc_id  char* to the document this word belongs in
 * @return the tf of this word in this doc_id
 */
int get_tf (struct wordNode* wordPtr, char* doc_id);

/**
 * Removes all words in hash table whose idf = 0.
 * @param ht pointer to the hashtable
 */
void stop_words (struct hashtable* ht);

/**
 * Takes a set of documents and populates the hashtable.
 * @param  ht    pointer to the hashtable
 */
void train (struct hashtable* ht);

/**
 * Reads in search query from user at console.
 * @param str        the string that needs to be split into separate words
 * @paran query_len  parameter will hold the length of the search_query
 * @return array of strings where each string is each search term
 */
char** read_query (char* str, int* query_len);

/**
 * Computes the tf-idf score for this document and givens set of search terms
 * @param  ht           pointer to the hashtable we are working with
 * @param  search_query string array of search terms
 * @param  query_len    length of the search query
 * @param  doc_id       char* to the doc_id we are computing tf-idf for
 * @return              the relevancy score of this doc_id against the given search_query
 */
double compute_tf_idf (struct hashtable* ht, char** search_query, int query_len, char* doc_id);

/**
 * Print the contents of the most relvant document to console and list all files and
 * their scores in order in search_scores.txt
 * @param  ht           pointer to the hashtable we are working with
 * @param scores array of relevancy_scores
 */
void output_results (struct hashtable* ht, struct relevancy_score** scores);

/**
 * Ranks the documents in order of their tf-idf scores and outputs results.
 * @param  ht            pointer to the hashtable
 * @param  search_query  string array of search terms
 * @param  query_len     length of the search query
 */
void rank (struct hashtable* ht, char** search_query, int query_len);

#endif

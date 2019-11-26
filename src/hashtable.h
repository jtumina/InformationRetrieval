/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Represents a hashtable data structure.
***************************************************************************************/

#ifndef hashtable_H
#define hashtable_H

struct docNode {
    char* doc_id;
    int tf;
    struct docNode* next;
};

struct wordNode {
        char* word;
        int df;
        struct docNode* docHead;
        struct wordNode* next;
};

struct hashtable {
        struct wordNode** map;
        int num_buckets;
        int num_elements;
        char** docIDs;
        int num_docs;
};

/**
 * Creates a new hashtable.
 * @param  num_buckets Number of buckets this hashtable should have
 * @return pointer to this hashtable
 */
struct hashtable* ht_create (int num_buckets);
/**
 * Initialize a new docNode with the given parameters.
 * @param docPtr  pointer to the docNode to edit
 * @param doc_id  char* to doc_id word belongs to
 */
void init_docNode (struct docNode* docPtr, char* doc_id);
/**
 * Initialize a new word node with the given parameters.
 * @param wordPtr pointer to the wordNode to edit
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 */
void init_wordNode (struct wordNode* wordPtr, char* word, char* doc_id);
/**
 * (1) Inserts this word and doc_id pair into hashtable along with the
 *     corresponding docNode for which this word occurs in.
 * (2) If the word and doc_id pair already exists, update its tf.
 * (3) If the word exists but in a different document,
 *     add new doc_id and update its df.
 * @param ht      pointer to the hashtable
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 */
void ht_insert (struct hashtable* ht, char* word, char* doc_id);
/**
 * Deallocate the given list of docNodes.
 * @param docPtr pointer to head of the list
 */
void destroy_docList (struct docNode* docPtr);
/**
 * Deallocate the given list of wordNodes.
 * @param wordPtr pointer to head of the list
 */
void destroy_wordList (struct wordNode* wordPtr);
/**
 * Deallocate this hashtable.
 * @param ht pointer to hashtable
 */
void ht_destroy (struct hashtable* ht);
/**
 * Hashing function to determine which bucket this word belongs in.
 * @param ht      pointer to the hashtable
 * @param word    char* to word we need to add
 * @return the hash code of this word and doc_id pair
 */
int hash_code (struct hashtable* ht, char* word);
/**
 * Searches the given hashtable for this word..
 * @param  ht      pointer to the hashtable to search in
 * @param  word    char* to the word to search for
 * @return pointer to the wordNode this word belongs
 */
int get_word (struct hashtable* ht, char* word);

#endif

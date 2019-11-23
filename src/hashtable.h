/*****************************************
* Jack Umina
* Created Nov, 2019
* Represents a hashtable data structure.
*****************************************/

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
        int idf;
        struct docNode* head;
        struct wordNode* next;
};

struct hashtable {
        struct wordNode** map;
        int num_buckets;
        int num_elements;
        char** docs;
        int num_docs;
};

/**
 * Creates a new hashtable.
 * @param  num_buckets Number of buckets this hashtable should have
 * @return pointer to this hashtable
 */
struct hashtable* ht_create (int num_buckets);
/**
 * (1) Inserts this word and doc_id pair into hashtable along with the
 *     corresponding docNode for which this word occurs in.
 * (2) If the word and doc_id pair already exists, update its tf.
 * (3) If the word exists but in a different document,
 *     add new doc_id and update its df.
 * @param ht      pointer to the hashmap
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 */
void ht_insert (struct hashtable* ht, char* word, char* doc_id);
/**
 * Remove this word and doc_id pair from hashtable.
 * Function is used to remove stop words.
 * @param ht      pointer to the hashmap
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 */
void ht_remove (struct hashtable* ht, char* word, char* doc_id);
/**
 * Deallocate this hashtable.
 * @param ht pointer to hashtable
 */
void ht_destroy (struct hashtable* ht);
/**
 * Hashing function to determine which bucket this word and doc_id belongs in.
 * @param ht      pointer to the hashmap
 * @param word    char* to word we need to add
 * @param doc_id  char* to doc_id word belongs to
 * @return the hash code of this word and doc_id pair
 */
int hash_code (struct hashtable* ht, char* word, char* doc_id);

#endif

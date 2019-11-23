/*****************************************
* Jack Umina
* Created Nov, 2019
* Represents a hashtable data structure.
*****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

/**
 * Allocates a new HashMap with the specified number of buckets.
 * @param num_buckets      number of buckets to create
 * @return struct hasmap*  pointer to a hashmap
 */
struct hashmap* hm_create(int num_buckets) {
    // Initialize space for hm
	struct hashmap* hm = (struct hashmap*) malloc (sizeof (struct hashmap));

    hm->map = (struct llnode**) malloc (num_buckets * sizeof (struct llnode*));
	hm->num_buckets = num_buckets;
	hm->num_elements = 0;

    // Initialize space for a list pointer in each bucket
	for (int i = 0; i < num_buckets; i++) {
		hm->map[i] = (struct llnode*) malloc (sizeof (struct llnode));

		// Initialize variables in llnode to NULL/0
		hm->map[i]->word = NULL;
		hm->map[i]->document_id = NULL;
		hm->map[i]->num_occurrences = 0;
		hm->map[i]->next = NULL;
	}

    return hm;
}

/**
 * Return the value associated with the key that is passed in within the HashMap
 * that is passed in. If the element is not found, return -1.
 * @param hm           pointer to hashmap
 * @param word         word to search for
 * @param document_id  id to which document
 * @return the num_occurrences of the word and document_id
 */
int hm_get(struct hashmap* hm, char* word, char* document_id) {
    // Get the bucket this word would be in
	int hashCode = hash (hm, word, document_id);

    // Set pointer to point to correct bucket
	struct llnode* nodePtr = hm->map[hashCode];

    // If the word is NULL in this bucket, the word does not exist in the list
    if (nodePtr->word == NULL) {
        return -1;
    }

    // While the word is not NULL and either does not equal the word we are trying to
    // insert or does not equal the document_id, move to next node in list
	while (nodePtr != NULL
			&& (strcmp (nodePtr->word, word) != 0 || strcmp (nodePtr->document_id, document_id) != 0)) {
		nodePtr = nodePtr->next;
	}

    // If we reached NULL, word does not exist
    if (nodePtr == NULL) {
        return -1;
    }

    // Else, we have found the word, return the num_occurrences of it
    return nodePtr->num_occurrences;
}

/**
 * Put the key value pair into hashmap.
 * If word and document_id already exist, update its num_occurrences
 * @param hm				pointer to hashmap
 * @param word				word to be stored
 * @param document_id		to which document
 * @param num_occurrences   of this word and document_id
 */
void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences){
    // Get this bucket this word belongs in
	int hashCode = hash (hm, word, document_id);

    // Set pointer to point to correct bucket
	struct llnode* nodePtr = hm->map[hashCode];

    // If this bucket is empty, set a node to this word and return
	if (nodePtr->word == NULL) {
		nodePtr->word = word;
		nodePtr->document_id = document_id;
		nodePtr->num_occurrences = num_occurrences;
        return;
	}

	// Temporary pointer to track node before the one we are looking at,
	// this way we can set the nodePtr->next to the correct node
	struct llnode* lastPtr = NULL;

    // While the word is not NULL and either does not equal the word we are trying to
    // insert or does not equal the document_id, move to next node in list
	while (nodePtr != NULL) {
		// If we've found the word already in hashmap, update its num_occurrences
		if (strcmp (nodePtr->word, word) == 0 && strcmp (nodePtr->document_id, document_id) == 0) {
			nodePtr->num_occurrences = num_occurrences;
			return;
		}
		// Else, continue on to next node
		lastPtr = nodePtr;
		nodePtr = nodePtr->next;
	}

    // Word is not in hashmap, allocate space for a new node
    nodePtr = (struct llnode*) malloc (sizeof (struct llnode));
    nodePtr->word = word;
    nodePtr->document_id = document_id;
    nodePtr->num_occurrences = num_occurrences;
    nodePtr->next = NULL;

	// Set node before the one's next to this node
	lastPtr->next = nodePtr;

	// Increment num_elements
	hm->num_elements--;
}

/**
 * Removes the key value pair associated with the given key
 * @param hm			pointer to hashmap
 * @param word			to remove
 * @param document_id   this word is in
 */
void hm_remove(struct hashmap* hm, char* word, char* document_id) {
	// Get the bucket this word would be in
	int hashCode = hash (hm, word, document_id);

    // Set pointer to point to correct bucket
	struct llnode* nodePtr = hm->map[hashCode];
	// Pointer to track node before nodePtr (so we can stich list together after removal)
	struct llnode* temp = NULL;

    // If the word is NULL in this bucket, the word does not exist in the list
    if (nodePtr->word == NULL) {
        return;
    }

    // While the word is not NULL and either does not equal the word we are trying to
    // insert or does not equal the document_id, move to next node in list
	while (nodePtr != NULL) {

		// If we've found the word already in hashmap, remove it
		if (strcmp (nodePtr->word, word) == 0 && strcmp (nodePtr->document_id, document_id) == 0) {

			// If this isn't the head of list, stich the adjacent nodes together
			if (temp != NULL) {
				temp->next = nodePtr->next;
			}
			// Now free the node
			free (nodePtr);
			// Decrement num_elements
			hm->num_elements--;

			return;
		}
		temp = nodePtr;
		nodePtr = nodePtr->next;
	}
}

/**
 * Deallocate the hashmap and all of its elements
 * @param hm   pointer to the hashmap
 */
void hm_destroy(struct hashmap* hm){
	// Loop through each bucket
	for (int i = 0; i < hm->num_buckets; i++) {

		// Points to head of the list
		struct llnode* head = hm->map[i];
		// Temporary node pointer
		struct llnode* temp = NULL;

		// Loop through each node in the list by setting head to the next
		// node and then freeing the current one
		while (head != NULL) {
			temp = head;
			head = head->next;
			free (temp);
		}
	}
	free (hm);
}

/**
 * Take the given word and document_id and map them to a bucket in the HashMap.
 * @param hm pointer to hashmap
 * @param word         key
 * @param document_id  id to which document
 */
int hash(struct hashmap* hm, char* word, char* document_id){
	// Track sum of ASCII values of word
	int sum = 0;
	int i = 0;

	// Loop through, adding ASCII values of each char to sum
	while (*(word+i) != '\0') {
		sum += (int) *(word+i);
		i++;
	}

	i = 0;
	// Now loop through document_id and do the same
	while (*(document_id+i) != '\0') {
		sum += (int) *(document_id+i);
		i++;
	}

	// Return the sum % num_buckets to keep range consitent
	return sum % hm->num_buckets;
}

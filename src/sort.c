/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Handles sorting of relevancy_score structs.
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"
#include "infoRetrieval.h"
#include "sort.h"

/**
 * Function to compare to relevancy_score structs according to their scores.
 * @param  a relevancy_score 1
 * @param  b relevancy_score 2
 * @return negative number if a->score > b->score
 *         positive number if a->score < b->score
 *         and 0 if a->score = b->score
 */
int comparator (const void* a, const void* b)
{
    // Since comparator() requires an int to be returned, need to multiply scores by 1 million
    // to get an int that we can compare
    int s1 = (int) ( ((struct relevancy_score*) a)->score * 1000000 );
    int s2 = (int) ( ((struct relevancy_score*) b)->score * 1000000 );
    return (s2 - s1);
}

/**
 * Function to sort an array of relevancy_score structs.
 * @param ht      pointer to the hashtable
 * @param scores array to sort
 */
void sort (struct hashtable* ht, struct relevancy_score* scores) {
    qsort ((void*) scores, ht->num_docs, sizeof (struct relevancy_score), comparator);
}

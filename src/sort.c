/**************************************************************************************
* Jack Umina
* Created Nov, 2019
* Handles sorting of relevancy_score structs.
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "infoRetrieval.h"

/**
 * Function to compare to relevancy_score structs according to their scores.
 * @param  a relevancy_score 1
 * @param  b relevancy_score 2
 * @return positive number if a->score > b->score
 *         negative number if a->score < b->score
 *         and 0 if a->score = b->score
 */
int comparator (const void *a, const void *b)
{
    int s1 = (int) ((struct relevancy_score *) a)->score;
    int s2 = (int) ((struct relevancy_score *) b)->score;
    return (s1 - s2);
}

/**
 * Function to sort an array of relevancy_score structs.
 * @param scores array to sort
 */
void sort (struct relevancy_score** scores) {
    // Get length of scores
    int scores_len = (int) (sizeof (scores) / sizeof (scores[0]));

    qsort ((void*) scores, scores_len, sizeof (scores[0]), comparator);
}

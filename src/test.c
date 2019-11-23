#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

int readFile (char* fileName, struct hashmap* hm);

int main(void){

	// Array of all docs to be added to hashmap
	char documents[3][7] = {"D1.txt", "D2.txt", "D3.txt"};
	// char documents[1][7] = {"D3.txt"};

	// Create the hashmap
	struct hashmap* hm = hm_create (7);

	// Get length of documents
	int len = (int) (sizeof (documents) / sizeof (char*));

	// Read each file, checking if there was an error accessing a file
	printf ("\nBegin reading documents:\n");
	for (int i = 0; i < len+1; i++) {
	    if (!readFile (*(documents+i), hm)) {
			printf ("Error occured while attempting to access file: %s.\n", *(documents+i));
			return 0;
		}
	}
	printf ("\n");

    printf ("num_occurrences of 'homework' in D3: %d\n", hm_get (hm, "homework", "D3.txt"));
    printf ("num_occurrences of 'love' in D1: %d\n", hm_get (hm, "love", "D1.txt"));
    printf ("num_occurrences of 'rocks' in D2: %d\n", hm_get (hm, "rocks", "D2.txt"));
	
	printf ("\nRemoving 'homework' from D3...\n");
	hm_remove (hm, "homework", "D3.txt");
	printf ("num_occurrences of 'homework' in D3: %d\n", hm_get (hm, "homework", "D3.txt"));

	printf ("\n");

	// Free hashmap
	hm_destroy (hm);
}

/**
 * Reads file and adds words to hashmap
 * @param fileName to be read from
 * @param hm pointer to HashMap
 * @return 1 if succesful
 */
int readFile (char* fileName, struct hashmap* hm) {

	// Open file in "read" mode ("r") and declare char array to hold each line
	FILE* f = fopen (fileName, "r");

	// If file could not be opened, return
    if (f == NULL) {
        return 0;
    }

	int maxLineLen = 200;
	char* line = (char*) malloc (maxLineLen * sizeof (char));
    char* word;

    // Loop through file line by line
	while (fgets (line, maxLineLen, f) != NULL) {

		// Print line for testing purposes
		printf ("%s: %s", fileName, line);

        // Remove \n from line
        strtok (line, "\n");

        // Get first word from line,
        // then continue to get each word untill NULL
        word = strtok (line, " ");
        while (word != NULL) {

            // Check if word exists in hashmap
            int num_occurrences = hm_get (hm, word, fileName);
            if (num_occurrences == -1) {
                hm_put (hm, word, fileName, 1);
            } else { // Increment num_occurrences for word
                hm_put (hm, word, fileName, num_occurrences+1);
            }

            // Get next word
            word = strtok (NULL, " ");
        }
    }
	fclose (f);
    return 1;
}

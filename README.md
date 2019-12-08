# InformationRetrieval
Application searches a directory of text files called `p5docs/` for a given search query and prints the most relevant document to the console
and outputs the other sorted file names and their associated relevancy scores to a file called `search_scores.txt`.

## Compliling and Running
Project can be compiled using command `make` followed by running the executable with the following arguments:
`./search <argv[1]> <argv[2]>`
where `argv[1]` is a integer specifying the number of buckets the hashtable should have and arg[2] is a single string specifying the search query.

## Requirements
1. No word in the search query or any of the text files can be longer than 20 characters.
2. The text files must be plain english characters with no punctutation.

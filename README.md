# InformationRetrieval
Implements TF-IDF algorithm in C to rank sets of text files according to their relevancy to a search query.

## TODO List:
#### Finish ```rank()```
- [X] Split up calculating tf-idf into different function
- [X] Sort the list of relevancy scores and thus, sort doc_ids
    - [x] Implement sort using ```qsort()```
    - [ ] Create an ```output_results()``` function to print the highest ranked doc to screen and to output scores to file

#### Finish ```train()```
- [ ] Maybe method to read file?

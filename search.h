#ifndef SEARCH_H
#define SEARCH_H

#include "index.h"  // Include FileData structure definition

// -------------------------------------------------------------
// Function Declarations for search.c
// -------------------------------------------------------------

// Searches for a single word (case-insensitive)
void searchWord(struct FileData *head, const char *query);

// Searches for a phrase (two or more words together)
void searchPhrase(struct FileData *head, const char *phrase);

// Searches for multiple words entered separated by spaces.
// For each word prints occurrences and totals.
void searchMultipleWords(struct FileData *head, const char *query);

#endif

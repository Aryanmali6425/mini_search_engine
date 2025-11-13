#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "search.h"

/* ----------------------------------------------------
   Helper function: Convert string to lowercase
   (Used for case-insensitive matching)
---------------------------------------------------- */
static void toLowerCopy(char *dest, const char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

/* ----------------------------------------------------
   WORD SEARCH
   Searches for a single word in all indexed lines.
---------------------------------------------------- */
void searchWord(struct FileData *head, const char *query) {
    if (!query || strlen(query) == 0) {
        printf("  Empty query entered.\n");
        return;
    }

    char lowerQuery[128];
    toLowerCopy(lowerQuery, query);

    printf("\nSearching for word: \"%s\"\n", query);
    printf("----------------------------------------------\n");

    int found = 0, totalMatches = 0;

    for (struct FileData *cur = head; cur != NULL; cur = cur->next) {
        char lineCopy[MAX_LINE];
        char lowerLine[MAX_LINE];

        strncpy(lineCopy, cur->line, MAX_LINE - 1);
        lineCopy[MAX_LINE - 1] = '\0';

        toLowerCopy(lowerLine, lineCopy);

        // Tokenize line to separate words
        const char *delims = " \t\r\n,.;:!?()[]{}<>\"'/-_\\|@#%^&*+=~`";
        char *token = strtok(lowerLine, delims);
        int matchedInThisLine = 0;

        while (token != NULL) {
            if (strcmp(token, lowerQuery) == 0) {
                if (!matchedInThisLine) {
                    printf(" %-20s (Line %3d): %s\n",
                           cur->fileName, cur->lineNumber, cur->line);
                    matchedInThisLine = 1;
                }
                totalMatches++;
                found = 1;
            }
            token = strtok(NULL, delims);
        }
    }

    if (!found)
        printf(" Word not found in any file.\n");
    else
        printf("----------------------------------------------\nTotal matches found: %d\n", totalMatches);
}

/* ----------------------------------------------------
   PHRASE SEARCH
   Finds an exact phrase (2 or more words together)
---------------------------------------------------- */
void searchPhrase(struct FileData *head, const char *phrase) {
    if (!phrase || strlen(phrase) == 0) {
        printf("  Empty phrase entered.\n");
        return;
    }

    char lowerPhrase[256];
    toLowerCopy(lowerPhrase, phrase);

    printf("\n Searching for phrase: \"%s\"\n", phrase);
    printf("----------------------------------------------\n");

    int found = 0, totalLines = 0;

    for (struct FileData *cur = head; cur != NULL; cur = cur->next) {
        char lowerLine[MAX_LINE];
        toLowerCopy(lowerLine, cur->line);

        if (strstr(lowerLine, lowerPhrase) != NULL) {
            printf(" %-20s (Line %3d): %s\n",
                   cur->fileName, cur->lineNumber, cur->line);
            found = 1;
            totalLines++;
        }
    }

    if (!found)
        printf(" Phrase not found in any file.\n");
    else
        printf("----------------------------------------------\n Total lines matched: %d\n", totalLines);
}

/* ----------------------------------------------------
   MULTI-WORD SEARCH
   User can type several words separated by spaces.
   For each word, we print occurrences and a small summary.
---------------------------------------------------- */
void searchMultipleWords(struct FileData *head, const char *query) {
    if (!query || strlen(query) == 0) {
        printf("  Empty query entered.\n");
        return;
    }

    // Make a mutable copy
    char qcopy[512];
    strncpy(qcopy, query, sizeof(qcopy)-1);
    qcopy[sizeof(qcopy)-1] = '\0';

    // Tokenize query by spaces (treat each token as a separate word)
    const char *spaceDelim = " \t\r\n";
    char *words[50];
    int wordCount = 0;
    char *tk = strtok(qcopy, spaceDelim);
    while (tk && wordCount < 50) {
        // trim empty tokens
        if (strlen(tk) > 0) {
            words[wordCount++] = strdup(tk); // remember to free
        }
        tk = strtok(NULL, spaceDelim);
    }

    if (wordCount == 0) {
        printf(" No valid words entered.\n");
        return;
    }

    printf("\nSearching for multiple words (%d):\n", wordCount);
    for (int i = 0; i < wordCount; ++i) {
        char lowerQuery[256];
        toLowerCopy(lowerQuery, words[i]);
        printf("\n>> Word: \"%s\"\n", words[i]);
        printf("----------------------------------------------\n");

        int found = 0, totalMatches = 0;
        for (struct FileData *cur = head; cur != NULL; cur = cur->next) {
            char lineCopy[MAX_LINE];
            char lowerLine[MAX_LINE];

            strncpy(lineCopy, cur->line, MAX_LINE - 1);
            lineCopy[MAX_LINE - 1] = '\0';
            toLowerCopy(lowerLine, lineCopy);

            const char *delims = " \t\r\n,.;:!?()[]{}<>\"'/-_\\|@#%^&*+=~`";
            char *token = strtok(lowerLine, delims);
            int matchedInThisLine = 0;
            while (token != NULL) {
                if (strcmp(token, lowerQuery) == 0) {
                    if (!matchedInThisLine) {
                        printf(" %-20s (Line %3d): %s\n",
                               cur->fileName, cur->lineNumber, cur->line);
                        matchedInThisLine = 1;
                    }
                    totalMatches++;
                    found = 1;
                }
                token = strtok(NULL, delims);
            }
        }

        if (!found)
            printf(" Not found.\n");
        else
            printf("----------------------------------------------\n Total matches for \"%s\": %d\n", words[i], totalMatches);

        free(words[i]);
    }
}

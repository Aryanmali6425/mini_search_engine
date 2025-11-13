#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>     // For folder and file listing
#include "index.h"

/* ----------------------------------------------
   Convert a string to lowercase (for easy matching)
---------------------------------------------- */
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = (char)tolower((unsigned char)str[i]);
}

/* ----------------------------------------------
   Add a new line (record) to the linked list
---------------------------------------------- */
void addToList(struct FileData **head, const char *fileName, int lineNumber, const char *line) {
    struct FileData *newNode = (struct FileData *)malloc(sizeof(struct FileData));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed!\n");
        return;
    }

    strncpy(newNode->fileName, fileName, MAX_FILE_NAME - 1);
    newNode->fileName[MAX_FILE_NAME - 1] = '\0';

    newNode->lineNumber = lineNumber;

    strncpy(newNode->line, line, MAX_LINE - 1);
    newNode->line[MAX_LINE - 1] = '\0';

    // Insert new node at the beginning of the list
    newNode->next = *head;
    *head = newNode;
}

/* -------------------------
   File statistics storage
   ------------------------- */
static struct FileInfo *statsHead = NULL;
static int totalWords = 0;

static void addFileInfo(const char *fileName, int lines, int words) {
    struct FileInfo *n = (struct FileInfo *)malloc(sizeof(struct FileInfo));
    if (!n) return;
    strncpy(n->fileName, fileName, MAX_FILE_NAME - 1);
    n->fileName[MAX_FILE_NAME - 1] = '\0';
    n->lines = lines;
    n->words = words;
    n->next = statsHead;
    statsHead = n;
}

/* ----------------------------------------------
   Build the index by reading all .txt files
   from the given folder path
---------------------------------------------- */
void buildIndex(struct FileData **head, const char *folderPath) {
    // clear old index list before building if present
    if (*head) freeList(*head);
    // clear old stats
    clearStats();

    DIR *folder = opendir(folderPath);
    if (!folder) {
        fprintf(stderr, " Error: Cannot open folder '%s'\n", folderPath);
        return;
    }

    struct dirent *entry;
    char filePath[1024];
    int fileCount = 0, totalLines = 0;

    printf("\n Scanning folder: %s\n", folderPath);

    while ((entry = readdir(folder)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Only process .txt files
        const char *ext = strrchr(entry->d_name, '.');
        if (!ext || strcmp(ext, ".txt") != 0)
            continue;

        snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

        FILE *fp = fopen(filePath, "r");
        if (!fp) {
            fprintf(stderr, "  Could not open file: %s\n", filePath);
            continue;
        }

        char line[MAX_LINE];
        int lineNo = 0;
        int fileWordCount = 0;
        // tokenization delimiters - keep comprehensive set
        const char *delims = " \t\r\n,.;:!?()[]{}<>\"'/-_\\|@#%^&*+=~`";

        while (fgets(line, sizeof(line), fp)) {
            lineNo++;
            // Remove newline characters
            line[strcspn(line, "\r\n")] = '\0';
            addToList(head, entry->d_name, lineNo, line);
            totalLines++;

            // count words in this line
            char temp[MAX_LINE];
            strncpy(temp, line, sizeof(temp) - 1);
            temp[sizeof(temp) - 1] = '\0';

            // tokenize to count words (simple approach)
            char *tok = strtok(temp, delims);
            while (tok) {
                fileWordCount++;
                tok = strtok(NULL, delims);
            }
        }

        fclose(fp);
        fileCount++;
        totalWords += fileWordCount;
        addFileInfo(entry->d_name, lineNo, fileWordCount);
        printf(" Indexed: %s (%d lines, %d words)\n", entry->d_name, lineNo, fileWordCount);
    }

    closedir(folder);

    if (fileCount == 0)
        printf("  No .txt files found in '%s'.\n", folderPath);
    else
        printf("\n Indexed %d file(s), %d total lines, %d total words.\n", fileCount, totalLines, totalWords);

    printf("----------------------------------------------\n\n");
}

/* ----------------------------------------------
   Print file statistics
---------------------------------------------- */
void printFileStats(void) {
    if (!statsHead) {
        printf("\n No file statistics available. Build the index first.\n");
        return;
    }
    printf("\nFile statistics:\n");
    printf("----------------------------------------------\n");
    struct FileInfo *t = statsHead;
    int totalFiles = 0;
    int totalLines = 0;
    int totalW = 0;
    while (t) {
        printf(" %-30s : %4d lines, %6d words\n", t->fileName, t->lines, t->words);
        totalFiles++;
        totalLines += t->lines;
        totalW += t->words;
        t = t->next;
    }
    printf("----------------------------------------------\n");
    printf(" Total files: %d | Total lines: %d | Total words: %d\n", totalFiles, totalLines, totalW);
}

/* ----------------------------------------------
   Return total words in corpus
---------------------------------------------- */
int getTotalWords(void) {
    return totalWords;
}

/* ----------------------------------------------
   Free stats list and reset counters
---------------------------------------------- */
void clearStats(void) {
    struct FileInfo *t;
    while (statsHead) {
        t = statsHead;
        statsHead = statsHead->next;
        free(t);
    }
    totalWords = 0;
}

/* ----------------------------------------------
   Free the entire linked list from memory
---------------------------------------------- */
void freeList(struct FileData *head) {
    struct FileData *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

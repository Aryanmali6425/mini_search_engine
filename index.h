#ifndef INDEX_H
#define INDEX_H

// ------------------- CONSTANTS -------------------
#define MAX_LINE 4096         // increase to support more complex/long lines
#define MAX_FILE_NAME 260     // Maximum file name length

// ------------------- STRUCTURE -------------------
// Each node stores one line of text, its file name and line number
struct FileData {
    char fileName[MAX_FILE_NAME];  // Name of the text file
    int lineNumber;                // Line number in the file
    char line[MAX_LINE];           // Actual text line
    struct FileData *next;         // Pointer to next line (linked list)
};

// Per-file statistics
struct FileInfo {
    char fileName[MAX_FILE_NAME];
    int lines;
    int words;
    struct FileInfo *next;
};

// ------------------- FUNCTION DECLARATIONS -------------------

// Convert a string to lowercase (used for case-insensitive matching)
void toLowerCase(char *str);

// Add a line (record) to the linked list
void addToList(struct FileData **head, const char *fileName, int lineNumber, const char *line);

// Build the index by reading all .txt files from a folder
void buildIndex(struct FileData **head, const char *folderPath);

// Free the memory used by the linked list
void freeList(struct FileData *head);

// File statistics helpers
void printFileStats(void);      // print per-file stats
int getTotalWords(void);        // get total words in corpus
void clearStats(void);          // free stats list and reset counters

#endif

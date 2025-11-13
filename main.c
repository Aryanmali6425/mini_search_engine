#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "search.h"

void printHeader() {
    printf("==============================================\n");
    printf("           MINI SEARCH ENGINE IN C\n");
    printf("==============================================\n");
}

void printMenu() {
    printf("\n----------------------------------------------\n");
    printf("                 MAIN MENU\n");
    printf("----------------------------------------------\n");
    printf("1. Build / Rebuild Index\n");
    printf("2. Search for a Word\n");
    printf("3. Search for a Phrase\n");
    printf("4. Show Index summary\n");
    printf("5. File statistics (per-file lines/words)\n");
    printf("6. Search for multiple words\n");
    printf("7. Show total words in corpus\n");
    printf("0. Exit\n");
    printf("----------------------------------------------\n");
    printf("Enter your choice: ");
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main() {
    struct FileData *head = NULL;
    int choice;
    int indexedOnce = 0;
    char folderPath[100] = "data";

    printHeader();

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf("\n Building index from folder: %s\n", folderPath);
                buildIndex(&head, folderPath);
                indexedOnce = 1;
                printf(" Index built successfully!\n");
                break;

            case 2:
                if (!indexedOnce) {
                    printf(" Please build the index first (Option 1).\n");
                    break;
                } {
                    char word[100];
                    printf("Enter a word to search: ");
                    fgets(word, sizeof(word), stdin);
                    word[strcspn(word, "\n")] = 0; // remove newline
                    searchWord(head, word);
                }
                break;

            case 3:
                if (!indexedOnce) {
                    printf(" Please build the index first (Option 1).\n");
                    break;
                } {
                    char phrase[256];
                    printf("Enter a phrase to search: ");
                    fgets(phrase, sizeof(phrase), stdin);
                    phrase[strcspn(phrase, "\n")] = 0;
                    searchPhrase(head, phrase);
                }
                break;

            case 4: {
                if (!indexedOnce) {
                    printf(" Please build the index first (Option 1).\n");
                    break;
                }
                int fileCount = 0, lineCount = 0;
                struct FileData *temp = head;
                char lastFile[100] = "";
                while (temp) {
                    if (strcmp(lastFile, temp->fileName) != 0) {
                        strcpy(lastFile, temp->fileName);
                        fileCount++;
                    }
                    lineCount++;
                    temp = temp->next;
                }
                printf("\nIndex:\n");
                printf(" - Total files indexed: %d\n", fileCount);
                printf(" - Total lines indexed: %d\n", lineCount);
                break;
            }

            case 5:
                if (!indexedOnce) {
                    printf(" Please build the index first (Option 1).\n");
                    break;
                }
                printFileStats();
                break;

            case 6:
                if (!indexedOnce) {
                    printf(" Please build the index first (Option 1).\n");
                    break;
                } {
                    char query[256];
                    printf("Enter multiple words (space separated): ");
                    fgets(query, sizeof(query), stdin);
                    query[strcspn(query, "\n")] = 0;
                    searchMultipleWords(head, query);
                }
                break;

            case 7:
                if (!indexedOnce) {
                    printf(" Please build the index first (Option 1).\n");
                    break;
                }
                printf("\nTotal words in indexed corpus: %d\n", getTotalWords());
                break;

            case 0:
                printf("\nExiting program...\n");
                freeList(head);
                clearStats();
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

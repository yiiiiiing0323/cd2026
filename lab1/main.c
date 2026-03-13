#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    int count; // Keep for HW#0 logic
    struct Node* next;
} Node;

Node* createNode(char c) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->data = c;
        newNode->count = 1;
        newNode->next = NULL;
    }
    return newNode;
}

int main() {
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        return 1;
    }

    Node* head = NULL;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        Node* current = head;
        Node* prev = NULL;
        int found = 0;

        while (current != NULL) {
            if (current->data == (char)ch) {
                current->count++;
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found) {
            Node* newNode = createNode((char)ch);
            if (head == NULL) {
                head = newNode;
            } else {
                prev->next = newNode;
            }
        }
    }

    fclose(fp);

    /* Lab#1 Output Requirement: #, i, n, c, u, ..... */
    Node* current = head;
    while (current != NULL) {
        if (current->data == '\n') {
            printf("\\n");
        } else if (current->data == '\t') {
            printf("\\t");
        } else if (current->data == ' ') {
            printf("' '");
        } else {
            printf("%c", current->data);
        }

        /* Print comma separator except for the last element */
        if (current->next != NULL) {
            printf(", ");
        }
        
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    printf("\n");

    return 0;
}

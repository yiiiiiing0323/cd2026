#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char data;
    int count;
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

    Node* current = head;
    while (current != NULL) {
        if (current->data == '\n') {
            printf("\\n : %d\n", current->count);
        } else if (current->data == '\r') {
            printf("\\r : %d\n", current->count);
        } else if (current->data == ' ') {
            printf("' ' : %d\n", current->count);
        } else {
            printf("%c : %d\n", current->data, current->count);
        }
        
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}

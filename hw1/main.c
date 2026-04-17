#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void print_token(const char* value, const char* token_name) {
    printf("%s: %s\n", value, token_name);
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

int main() {
    FILE *fp = fopen("main.c", "r");//開啟檔案名 
    if (fp == NULL) return 1;

    int c;
    char buffer[256];
    int idx;

    while ((c = fgetc(fp)) != EOF) {
        /* Skip whitespaces */
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            continue;
        }

        /* Identifier or Keyword  */
        if (is_alpha(c)) {
            idx = 0;
            buffer[idx++] = (char)c;
            while ((c = fgetc(fp)) != EOF && is_alnum(c)) {
                buffer[idx++] = (char)c;
            }
            buffer[idx] = '\0';
            ungetc(c, fp);

            if (strcmp(buffer, "int") == 0) print_token(buffer, "TYPE_TOKEN");
            else if (strcmp(buffer, "main") == 0) print_token(buffer, "MAIN_TOKEN");
            else if (strcmp(buffer, "if") == 0) print_token(buffer, "IF_TOKEN");
            else if (strcmp(buffer, "else") == 0) print_token(buffer, "ELSE_TOKEN");
            else if (strcmp(buffer, "while") == 0) print_token(buffer, "WHILE_TOKEN");
            else print_token(buffer, "ID_TOKEN");
        }
        /* Number Literal  */
        else if (is_digit(c)) {
            idx = 0;
            buffer[idx++] = (char)c;
            while ((c = fgetc(fp)) != EOF && is_digit(c)) {
                buffer[idx++] = (char)c;
            }
            buffer[idx] = '\0';
            ungetc(c, fp);
            print_token(buffer, "LITERAL_TOKEN");
        }
        /* Operators and Delimiters  */
        else {
            int next_c;
            switch (c) {
                case '=':
                    next_c = fgetc(fp);
                    if (next_c == '=') print_token("==", "EQUAL_TOKEN");
                    else { ungetc(next_c, fp); print_token("=", "ASSIGN_TOKEN"); }
                    break;
                case '>':
                    next_c = fgetc(fp);
                    if (next_c == '=') print_token(">=", "GREATEREQUAL_TOKEN");
                    else { ungetc(next_c, fp); print_token(">", "GREATER_TOKEN"); }
                    break;
                case '<':
                    next_c = fgetc(fp);
                    if (next_c == '=') print_token("<=", "LESSEQUAL_TOKEN");
                    else { ungetc(next_c, fp); print_token("<", "LESS_TOKEN"); }
                    break;
                case '+': print_token("+", "PLUS_TOKEN"); break;
                case '-': print_token("-", "MINUS_TOKEN"); break;
                case '(': print_token("(", "LEFTPAREN_TOKEN"); break;
                case ')': print_token(")", "REFTPAREN_TOKEN"); break;
                case '{': print_token("{", "LEFTBRACE_TOKEN"); break;
                case '}': print_token("}", "REFTBRACE_TOKEN"); break;
                case ';': print_token(";", "SEMICOLON_TOKEN"); break;
            }
        }
    }

    fclose(fp);
    return 0;
}

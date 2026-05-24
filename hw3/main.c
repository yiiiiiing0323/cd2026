#include <stdio.h>
#include <string.h>

/* 全域變數 */
char input_buffer[1024]; /* 儲存使用者輸入的數學式 */
int buffer_idx = 0;      /* 目前讀取字元的位置指標 */
int lookahead;
char token_value[256];   /* 儲存當前 NUM 的真實數字 */

/* Token 型態定義 */
#define NUM 256
#define PLUS '+'
#define LPAREN '('
#define RPAREN ')'
#define DONE -1

/* 函式原型宣告 */
int get_next_token();
void parse_S(int indent);
void parse_S_prime(int indent);
void parse_E(int indent);
void match(int expected);
void print_spaces(int count);

/* 輔助函式：印出指定數量的空格 */
void print_spaces(int count) {
    for (int i = 0; i < count; i++) {
        printf(" ");
    }
}

/* Scanner 邏輯：改從字串緩衝區讀取字元 */
int get_next_token() {
    int c;
    while ((c = input_buffer[buffer_idx++]) != '\0') {
        /* 忽略所有空白字元 */
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            continue;
        }
        
        /* 如果是數字，完整記錄到 token_value */
        if (c >= '0' && c <= '9') {
            int idx = 0;
            token_value[idx++] = (char)c;
            while ((c = input_buffer[buffer_idx]) >= '0' && c <= '9') {
                if (idx < 255) token_value[idx++] = (char)c;
                buffer_idx++;
            }
            token_value[idx] = '\0';
            return NUM;
        }
        
        /* 一般單一字元符號 */
        token_value[0] = (char)c;
        token_value[1] = '\0';
        return c;
    }
    return DONE;
}

void match(int expected) {
    if (lookahead == expected) {
        lookahead = get_next_token();
    } else {
        printf("\nParse Error: Expected %d\n", expected);
    }
}

/* ================= Parser 核心邏輯 ================= */

/* 規則：S -> E S' */
void parse_S(int indent) {
    printf("S -> E S'\n");
    
    /* 下一行 E 需要對齊的位置：目前的起始位置(indent) + "S -> " 的長度(5) */
    int next_E_indent = indent + 5;
    print_spaces(next_E_indent);
    parse_E(next_E_indent);
    
    /* 下一行 S' 需要對齊的位置：目前的起始位置(indent) + "S -> E " 的長度(7) */
    int next_S_prime_indent = indent + 7;
    print_spaces(next_S_prime_indent);
    parse_S_prime(next_S_prime_indent);
}

/* 規則：S' -> + S | epsilon */
void parse_S_prime(int indent) {
    if (lookahead == PLUS) {
        printf("S' -> + S\n");
        match(PLUS);
        
        /* 下一行 S 需要對齊的位置：目前的起點(indent) + "S' -> + " 的長度(9) */
        int next_S_indent = indent + 9;
        print_spaces(next_S_indent);
        parse_S(next_S_indent);
    } else {
        printf("S' -> epsilon\n");
    }
}

/* 規則：E -> num | ( S ) */
void parse_E(int indent) {
    if (lookahead == NUM) {
        printf("E -> %s\n", token_value);
        match(NUM);
    } else if (lookahead == LPAREN) {
        printf("E -> ( S )\n");
        match(LPAREN);
        
        /* 下一行 S 需要對齊的位置：目前的起點(indent) + "E -> ( " 的長度(7) */
        int next_S_indent = indent + 7;
        print_spaces(next_S_indent);
        parse_S(next_S_indent);
        match(RPAREN);
    } else {
        printf("Parse Error\n");
    }
}

int main() {
    /* 讓使用者直接輸入數學式 */
    printf("Please enter the expression to parse : ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1;
    }

    /* 移除 fgets 讀取到的換行符號 */
    unsigned long len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }

    buffer_idx = 0;
    lookahead = get_next_token();
    
    if (lookahead != DONE && strlen(input_buffer) > 0) {
        parse_S(0);
    } else {
        printf("Error: Input is empty.\n");
    }

    return 0;
}

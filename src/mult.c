#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 64

int main(void) {
    char buf[MAX_SIZE];
    int op1, op2;
    setbuf(stdout, NULL);
    while(!feof(stdin)) {
        memset(buf, 0, MAX_SIZE);
        fgets(buf, MAX_SIZE, stdin);
        sscanf(buf, "%d %d", &op1, &op2);
        printf("%d", op1 * op2);
    }
    return 0;
}
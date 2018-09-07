#include <stdio.h>

int main(void) {

    printf("Quel age avez-vous ?\n");

    while(1) {
        int age;
        scanf("%d", &age);
        ++age;
        printf("Vous avez %d ans", age);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL 20

/*********************************/
/* String Array (string_array_t) */
/*********************************/

typedef struct {
    char** content;
    size_t size;
    size_t allocated;
} string_array_t;

void check_alloc(void* ptr) {
    if(ptr == NULL) {
        perror("Memory error");
        exit(EXIT_FAILURE);
    }
}

string_array_t* string_array_new() {
    string_array_t* ptr = calloc(1, sizeof(string_array_t));
    check_alloc(ptr);
    return ptr;
}

void string_array_free(string_array_t* arr) {
    free(arr->content);
    free(arr);
}

void string_array_expand(string_array_t* arr) {
    if(arr->allocated == 0) {
        arr->allocated = 2;
    }
    else {
        arr->allocated *= 2;
    }

    arr->content = realloc(arr->content, arr->allocated * sizeof(char*));
    check_alloc(arr->content);
}

void string_array_insert(string_array_t* arr, char* str) {
    if(++arr->size > arr->allocated) {
        string_array_expand(arr);
    }

    arr->content[arr->size - 1] = str;
}

int str_ptr_cmp(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);
}

void string_array_sort(string_array_t* arr) {
    qsort(arr->content, arr->size, sizeof(char*), &str_ptr_cmp);
}

void string_array_print(string_array_t* arr) {
    for(size_t i = 0; i != arr->size; ++i) {
        puts(arr->content[i]);
    }
}

/***********************/
/* Helper IO functions */
/***********************/

void skip_line() {
    int c;
    while ((c = getc(stdin)) != EOF && c != '\r' && c != '\n');
}

char* read_word() {
    char* word = malloc(MAXL + 1);

    fputs("Enter word: ", stdout);
    fflush(stdout);
    fgets(word, MAXL + 1, stdin);

    size_t lb_pos = strcspn(word, "\r\n");
    word[lb_pos] = '\0';

    if(lb_pos == MAXL) {
        // Line was longer than MAXL. We skip to the end of the line.
        skip_line();
    }

    if(*word == '\0' || ferror(stdin)) {
        free(word);
        return NULL;
    }

    return word;
}

string_array_t* read_words() {
    string_array_t* arr = string_array_new();
    char* word;

    while((word = read_word()) != NULL) {
        string_array_insert(arr, word);
    }

    return arr;
}

/**************************/
/* Main : CLI entry point */
/**************************/

int main(void) {
    string_array_t* arr = read_words();
    string_array_sort(arr);
    string_array_print(arr);
    string_array_free(arr);

    return EXIT_SUCCESS;
}

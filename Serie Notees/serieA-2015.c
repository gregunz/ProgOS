#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* A FAIRE : complétez le début de ce programme. */


#define N(x) (sizeof(x)/sizeof(*x))
#define CHAR_LENGTH_MAX 1024

typedef char* (*Fonction)(const char*);

char* copy(const char* s);
char* to_upper_case_all_a(const char* s);
char* split_at_third_space(const char* s);
char* append_hello(const char* s);

void iterate_over_functions(const char* inputs[], size_t nb_inputs,
                           Fonction my_functions[], size_t nb_functions);


int main() {
    const char* inputs[] = {
      "Hallo Anno! Cammant ca va ? Ahlala...",
      "aaaaa",
      " a a a a a ",
      "Une phrose sons oucun o"
    };

    /* A FAIRE : définissez ici my_functions. */
	Fonction my_functions[4] = {
		copy,
		to_upper_case_all_a,
		split_at_third_space,
		append_hello
	};

    iterate_over_functions(inputs, N(inputs),
                           my_functions, N(my_functions));

    return 0;
}

void iterate_over_functions(const char* inputs[], size_t nb_inputs,
                           Fonction my_functions[], size_t nb_functions)
{
	for(int j=0; j<nb_functions; ++j){
		Fonction f = my_functions[j];
		printf("applying transformation #%d:\n", j);
		for(int i=0; i<nb_inputs; ++i){
			const char* s = inputs[i];
			char* applied = f(s);
			printf("\t-> \"%s\"\n", applied);
			free(applied);
		}
	}
}
char* copy(const char* s)
{
	size_t len = strlen(s);
	char* cpy = calloc(strlen(s)+1, sizeof(char));
	if(cpy == NULL){
		return NULL;
	}
	cpy[len] = '\0';
	strncpy(cpy, s, len);
	return cpy;
}
char* to_upper_case_all_a(const char* s)
{
	char* cpy = copy(s);
	for(int i=0; i<strlen(cpy); ++i){
		char c = cpy[i];
		if(c == 'a'){
			cpy[i] = 'A';
		}
	}
	return cpy;
}
char* split_at_third_space(const char* s)
{
	char* cpy = copy(s);
	size_t space = 0;
	for(int i=0; i<strlen(cpy); ++i){
		char c = cpy[i];
		if(c == ' '){
			space += 1;
		}
		if(space >= 3){
			cpy[i] = '\0';
			return cpy;
		}

	}
	return cpy;
}
char* append_hello(const char* s)
{
	size_t len = strlen(s);

	char* cpy = copy(s);
	cpy = realloc(cpy, len+5+1);
	strncpy(cpy+len, "hello", 5);
	cpy[len+5] = '\0';
	return cpy;

}

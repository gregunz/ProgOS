#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define HASH_TABLE_LOAD_FACTOR 0.75

#define CSV_MAX_LINE_SIZE 1024
#define CSV_SEPARATOR ','

size_t free_count = 0;
size_t alloc_count = 0;

/* ======================================================================
 * Provided utility functions
 * ======================================================================
 */

/** ----------------------------------------------------------------------
 ** Ask for a filename
 **/
void ask_filename(const char* question, char filename[])
{
    size_t len = 0;
    do {
        fputs(question, stdout);
        fflush(stdout);
        fgets(filename, FILENAME_MAX + 1, stdin);
        filename[strcspn(filename, "\r\n")] = '\0';
        len = strlen(filename) - 1;
    } while (len < 1 && !feof(stdin) && !ferror(stdin));
}

/** ----------------------------------------------------------------------
 ** Ask for a filename to open in a given mode
 **/
FILE* ask_filename_and_open(const char* question, const char* mode)
{
    char filename[FILENAME_MAX + 1] = "";

    ask_filename(question, filename);
    FILE* retour = fopen(filename, mode);

    if (retour == NULL) {
        fprintf(stderr, "Oops, je n'ai pas réussi à ouvrir le fichier \"%s\" en mode \"%s\"!\n",
                filename, mode);
    }
    return retour;
}

/** ----------------------------------------------------------------------
 ** Ask for a size
 **/
size_t ask_size_t(const char* question)
{
    size_t v = 0;
    int res = 0;
    do {
        fputs(question, stdout);
        fflush(stdout);
        res = scanf("%zu", &v);
        while (!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
    } while (!feof(stdin) && !ferror(stdin) && res != 1 && v == 0);
    return v;
}

/* ======================================================================
 * Part I -- Hashtable
 * ======================================================================
 */

/** ----------------------------------------------------------------------
 ** Hash a string for a given hashtable size.
 ** See http://en.wikipedia.org/wiki/Jenkins_hash_function
 **/
size_t hash_function(const char* key, size_t size)
{
    size_t hash = 0;
    size_t key_len = strlen(key);
    for (size_t i = 0; i < key_len; ++i) {
        hash += (unsigned char) key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % size;
}

/* ****************************************
 * TODO : add your own code here.
 * **************************************** */

 struct bucket_tmp{
    const char* key;
    const void* value;
    struct bucket_tmp *next;
};

typedef struct bucket_tmp bucket;

typedef struct {
    size_t size;
    size_t quantity;
    bucket** buckets;
} Htable;

/** Prototypes **/

Htable* construct_Htable(size_t size);
void delete_Htable_and_content(Htable* table);
void add_Htable_value(Htable *table, const char* key, const void* value);
const void* get_Htable_value(Htable* table, const char* key);

/** create a bucket* given its key* and value* **/
bucket* create_bucket(const char* key, const void* value);
/** clear the content (key, value) in the table in case you need to use it again **/
void clear_Htable_content(Htable* table);
/** add a (key, value) to the chained list starting with bucket b (update if key is already stored) **/
void add_value_chained_list(bucket* b, const char* key, const void* value, size_t* quantity);
/** given the starting element of a chained list and a key,
    return the bucket having the same key and/or the last element of the list **/
bucket* find_bucket_chained_list(bucket* b, const char* key);


/** My Functions **/

Htable* construct_Htable( size_t size ){
    Htable* table;
    bucket** buckets;
    if( (table = malloc( sizeof(Htable) )) == NULL ||
        (buckets = calloc( sizeof(bucket*), size )) == NULL ){
        return NULL;
    }
	alloc_count++;
	alloc_count++;
    table->buckets = buckets;
    table->size = size;
    table->quantity = 0;
    for(int i=0; i < table->size; ++i){ // initialize (even if calloc "may" be doing it) buckets to NULL
        (table->buckets)[i] = NULL;
    }
    return table;
}

void clear_Htable_content(Htable* table){
    if(table != NULL && table->buckets != NULL){
        for(int i=0; i < table->size; ++i){
            bucket* b = table->buckets[i];
            table->buckets[i] = NULL;
            while(b != NULL){ // there is (at least) a bucket => free entire chained list
                bucket* next = b->next;
                free(b->key);
				free_count++;
                free(b->value);
				free_count++;
                free(b);
				free_count++;
                b = next;
            }
        }
        table->quantity = 0;
    }
}

void delete_Htable_and_content(Htable* table){
    if(table != NULL){
        if(table->buckets != NULL){
            if(table->quantity > 0){
                clear_Htable_content(table);
            }
            free(table->buckets);
				free_count++;
        }
        free(table);
				free_count++;
    }
}

bucket* create_bucket(const char* key, const void* value){
    bucket* b;
    if( (b = malloc( sizeof(bucket) )) == NULL){
        return NULL;
    }
	alloc_count++;
    b->key = key;
    b->value = value;
    b->next = NULL;
    return b;
}

void add_Htable_value(Htable* table, const char* key, const void* value){
    if( table != NULL && table->buckets != NULL){
        size_t pos = hash_function(key, table->size);
        bucket* b = table->buckets[pos];
        if(b == NULL){ // No elements (yet), let's ADD (key, value)
            table->buckets[pos] = create_bucket(key, value);
            ++(table->quantity);
        } else { // at least one element already
            add_value_chained_list( b, key, value, &(table->quantity) );
        }
    }
}

bucket* find_bucket_chained_list(bucket* b, const char* key){
    while(b->next != NULL && strcmp(b->key, key) != 0){
        b = b->next;
    }
    return b;
}

void add_value_chained_list(bucket* b, const char* key, const void* value, size_t* quantity){
    /* let's find element to update and/or reach end of the chained list */
    b = find_bucket_chained_list(b, key);
    if(strcmp(b->key, key) == 0){ // UPDATE value from given key
        b->value = value;
    }else{ // ADD new (key, value)
        b->next = create_bucket(key, value);
        ++(*quantity);
    }
}

const void* get_Htable_value(Htable* table, const char* key){
    if(table != NULL && table->buckets != NULL){
        size_t pos = hash_function(key, table->size);
        bucket* b = table->buckets[pos];
        if( b != NULL ){
            b = find_bucket_chained_list(b, key);
            if(strcmp(b->key, key) == 0){
                return b->value;
            }
        }
    }
    return NULL;
}

/* ======================================================================
 * Provided: CSV file parser
 * ======================================================================
 */

typedef char* csv_row;
typedef const char* csv_const_row;

/** ----------------------------------------------------------------------
 ** Allocate and read a CSV row from a file
 **/
csv_row read_row(FILE* f)
{
    char line[CSV_MAX_LINE_SIZE + 1] = "";
    fgets(line, CSV_MAX_LINE_SIZE, f);
    line[strcspn(line, "\r\n")] = '\0'; // remove trailing '\n'
    size_t len = strlen(line);

    assert(len < CSV_MAX_LINE_SIZE); /* We should not have lines that long...
                                      * Not handled in this homework anyway!
                                      * Should be properly handled in real-life appli.
                                      */

    csv_row row;
    if ((row = calloc(len + 1, sizeof(char))) == NULL) {
        return NULL;
    }
	alloc_count++;
    strncpy(row, line, len);

    return row;
}

/** ----------------------------------------------------------------------
 ** Write a CSV row to a file
 **/
void write_row(FILE* out, const csv_const_row row, size_t ignore_index)
{
    size_t len = strlen(row);
    size_t current_element = 0;
    for (size_t i = 0; i < len; ++i) {
        if (row[i] == CSV_SEPARATOR) {
            ++current_element;
        }
        if (current_element != ignore_index && (ignore_index != 0 || current_element != ignore_index + 1 || row[i] != CSV_SEPARATOR)) {
            fprintf(out, "%c", row[i]);
        }
    }
}

/** ----------------------------------------------------------------------
 ** Write 2 CSV rows side-by-side to a file
 **/
void write_rows(FILE* out, const csv_const_row row1, const csv_const_row row2, size_t ignore_index)
{
    write_row(out, row1, (size_t) -1);
    fprintf(out, "%c", CSV_SEPARATOR);
    write_row(out, row2, ignore_index);
    fprintf(out, "\n");
}

/** ----------------------------------------------------------------------
 ** Copy and return the i'th element in the row
 **/
char* row_element(const csv_const_row row, size_t index)
{
    size_t len = strlen(row);
    size_t start = 0, end = 0;
    size_t current_element = 0;
    for (size_t i = 0; i < len; ++i) {
        if (row[i] == CSV_SEPARATOR || i == len - 1) {
            ++current_element;
            if (current_element == index) {
                start = i + 1;
            } else if (current_element == index + 1) {
                end = (i == len - 1) ? len : i;
                break;
            }
        }
    }

    if (end > 0) { // success
        size_t elem_len = end - start;
        char* element;
        if ((element = calloc(elem_len + 1, sizeof(char))) == NULL) {
            return NULL;
        }
	alloc_count++;
        element[elem_len] = '\0';
        strncpy(element, &row[start], elem_len);
        return element;
    } else {
        return NULL;
    }
}


/* ======================================================================
 * Part II -- Hashjoin
 * ======================================================================
 */

/* ****************************************
 * TODO : add your own code here.
 * **************************************** */
/** Functions' prototypes **/

int hash_join(FILE* in1, FILE* in2, FILE* out, size_t col1, size_t col2, size_t memory_authorized);

/** -----------------------------------------------------------------------------------------------
    writes in output the join for every row of in2 whose key is the table
**/
void write_join(FILE* in2, FILE* out, size_t col2, Htable* table);

/** -----------------------------------------------------------------------------------------------
    writes in output the first (information-) row from the join of the two inputs return 0 if there
    were no errors and 1 otherwise
**/
int write_first_rows(FILE* in1, FILE* in2, FILE* out, size_t col2);

/** -----------------------------------------------------------------------------------------------
    add the row of in1 to the table and writes join when it's the end of file (if table gets full
    before adding all rows of in1, it writes the join with in2 to output and clear the table content
    to go on adding rows of in1)
**/
void add_row1_and_write_join(FILE* in1, FILE* in2, FILE* out, csv_const_row r1, size_t col1, size_t col2, Htable* table);



/** Functions' declarations **/

int hash_join(FILE* in1, FILE* in2, FILE* out, size_t col1, size_t col2, size_t memory_authorized){

    if( in1 == NULL || in2 == NULL || out == NULL ||
       memory_authorized < sizeof(bucket) || write_first_rows(in1, in2, out, col2) != 0 ){
        return 1;
    }

    Htable* table = construct_Htable( memory_authorized / sizeof(bucket) );
    csv_const_row r1;

    while( !feof(in1) && (r1 = read_row(in1)) != NULL ){
        add_row1_and_write_join(in1, in2, out, r1, col1, col2, table);
    }

    delete_Htable_and_content(table);

    return 0;
}

void add_row1_and_write_join(FILE* in1, FILE* in2, FILE* out, csv_const_row r1, size_t col1, size_t col2, Htable* table){
    if(strlen(r1) > 0){
        char* key = row_element(r1, col1);
        add_Htable_value(table, key, r1);

        if( table->quantity >= table->size * HASH_TABLE_LOAD_FACTOR ){
            /* we're not done with r1 but TABLE IS FULL so we loop on r2 to write our
               output and start again with an empty table and at the beginning of r2 */
            write_join(in2, out, col2, table);
            clear_Htable_content(table);
            fseek(in2, 0, SEEK_SET);
        }
    } else {
        /* at the last loop r1 is allocated but it's empty (and != NULL) */
        free(r1);
				free_count++;
    }
    if( feof(in1) && table->quantity > 0){
        /* we're done with input 1, so we write the join (if there are elements in table) */
        write_join(in2, out, col2, table);
    }
}

void write_join(FILE* in2, FILE* out, size_t col2, Htable* table){
    csv_const_row r2;
    while( !feof(in2) && (r2 = read_row(in2)) != NULL ){
        if(strlen(r2) > 0){
            char* key = row_element(r2, col2);
            csv_const_row value = get_Htable_value(table, key);
            if( value != NULL){
                write_rows(out, value, r2, col2);
            }
            free(r2);
				free_count++;
            free(key);
				free_count++;
        } else {
            /* at the last loop r2 is allocated but it's empty (and != NULL) */
            free(r2);
				free_count++;
        }
    }
}


int write_first_rows(FILE* in1, FILE* in2, FILE* out, size_t col2){
    csv_const_row first_row1,
                  first_row2;
    if( feof(in1) || feof(in2) ||
       (first_row1 = read_row(in1)) == NULL ||
       (first_row2 = read_row(in2)) == NULL )
        {
        return 1;
    }
    if(strlen(first_row1) == 0 || strlen(first_row2) == 0){
        free(first_row1);
				free_count++;
        free(first_row2);
				free_count++;
        return 1;
    }
    write_rows(out, first_row1, first_row2, col2);
    free(first_row1);
				free_count++;
    free(first_row2);
				free_count++;
    return 0;
}



/* ======================================================================
 * Provided: main()
 * ======================================================================
 */

int main(void)
{
        //*
    FILE* in1 = fopen("department.csv", "r");
    //FILE* in1 = ask_filename_and_open("Entrez le nom du premier fichier : ", "r");
    if (in1 == NULL) {
        return EXIT_FAILURE;
    }

    FILE* in2 = fopen("employe.csv", "r");
    //FILE* in2 = ask_filename_and_open("Entrez le nom du second  fichier : ", "r");
    if (in2 == NULL) {
        fclose(in1);
        return EXIT_FAILURE;
    }

    FILE* out = fopen("output_employee.csv", "w");
    //FILE* out = ask_filename_and_open("Entrez le nom du fichier où écrire le résultat : ", "w");
    if (out == NULL) {
        fclose(in1);
        fclose(in2);
        return EXIT_FAILURE;
    }


    //size_t col1 = ask_size_t("Entrez l'index de la colonne à joindre dans le premier fichier : "),
    size_t col1 = 1,
    //       col2 = ask_size_t("Entrez l'index de la colonne à joindre dans le second  fichier : ");
           col2 = 1;


    int success = hash_join(in1, in2, out, col1, col2, 5*sizeof(bucket)
                            //ask_size_t("Entrez le budget mémoire autorisé (en octets) :  ")
                            );
    fclose(in1);
    fclose(in2);
    fclose(out);

    in1 = fopen("countries.csv", "r");
    //FILE* in1 = ask_filename_and_open("Entrez le nom du premier fichier : ", "r");
    if (in1 == NULL) {
        return EXIT_FAILURE;
    }

    in2 = fopen("cities.csv", "r");
    //FILE* in2 = ask_filename_and_open("Entrez le nom du second  fichier : ", "r");
    if (in2 == NULL) {
        fclose(in1);
        return EXIT_FAILURE;
    }

    out = fopen("output_countries.csv", "w");
    //FILE* out = ask_filename_and_open("Entrez le nom du fichier où écrire le résultat : ", "w");
    if (out == NULL) {
        fclose(in1);
        fclose(in2);
        return EXIT_FAILURE;
    }


    //size_t col1 = ask_size_t("Entrez l'index de la colonne à joindre dans le premier fichier : "),
    col1 = 0,
    //       col2 = ask_size_t("Entrez l'index de la colonne à joindre dans le second  fichier : ");
           col2 = 0;


    success = success + hash_join(in1, in2, out, col1, col2, 5*sizeof(bucket)
                            //ask_size_t("Entrez le budget mémoire autorisé (en octets) :  ")
                            );
    fclose(in1);
    fclose(in2);
    fclose(out);

    printf("alloc count : %d\n", alloc_count);
    printf("free count : %d\n", free_count);

    return success;
    /*/
    // OFFICIAL MAIN
    FILE* in1 = ask_filename_and_open("Entrez le nom du premier fichier : ", "r");
    if (in1 == NULL) {
        return EXIT_FAILURE;
    }

    FILE* in2 = ask_filename_and_open("Entrez le nom du second  fichier : ", "r");
    if (in2 == NULL) {
        fclose(in1);
        return EXIT_FAILURE;
    }

    FILE* out = ask_filename_and_open("Entrez le nom du fichier où écrire le résultat : ", "w");
    if (out == NULL) {
        fclose(in1);
        fclose(in2);
        return EXIT_FAILURE;
    }


    size_t col1 = ask_size_t("Entrez l'index de la colonne à joindre dans le premier fichier : "),
           col2 = ask_size_t("Entrez l'index de la colonne à joindre dans le second  fichier : ");


    int success = hash_join(in1, in2, out, col1, col2,
                            ask_size_t("Entrez le budget mémoire autorisé (en octets) :  ")
                            );
    fclose(in1);
    fclose(in2);
    fclose(out);

    return success;
    //*/
}

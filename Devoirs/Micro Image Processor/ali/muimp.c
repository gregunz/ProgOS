#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define OPEN_OK 0
#define OPEN_ERROR 1

#define MAX_IMAGE_WIDTH 100
#define MAX_IMAGE_HEIGHT 100

#define SYMBOLE_ZERO '.'
#define SYMBOLE_ONE '+'
#define SYMBOLE_ELSE '*'


const char SYMBOLS[3] = {'.', '+', '*'};

typedef double Pixel;
typedef struct {
    Pixel pixels[MAX_IMAGE_WIDTH][MAX_IMAGE_HEIGHT];
    int w, h;
} Image;

/*
 * Retourne le minimum des 2 nombres donnés en argument
 */
int min(int n1, int n2);

/*
 * Extrait un caractère unique qui doit se trouver dans la liste représentée par allowed
 */
int getChar(FILE* flot, const char* info, const char allowed[], size_t const taille, char* toUpdate);

/*
 * Extrait une information entière du flot donné et met à jour la variable transmise par son pointeur.
 * Retourne 0 si l'extraction s'est déroulée correctement, 1 si la lecture n'a pas pu se faire,
 * ou 2 si la lecture s'est faite mais que la valeur n'est pas comprise dans l'intervalle désiré.
 */
int getInt(FILE* flot, const char* info, const int lower_bound, const int upper_bound, int* toUpdate);

/*
 * Extrait une information réelle du flot donné et met à jour la variable transmise par son pointeur.
 * Retourne 0 si l'extraction s'est déroulée correctement, 1 si la lecture n'a pas pu se faire,
 * ou 2 si la lecture s'est faite mais que la valeur n'est pas comprise dans l'intervalle désiré.
 */
int getDouble(FILE* flot, const char* info, const double lower_bound, const double upper_bound, double* toUpdate);

/*
 * Demande un entier à l'utilisateur. Continue de demander tant que la lecture n'a pas pu
 * se faire ou tant que la valeur n'est pas comprise dans l'intervalle désiré.
 */
int askInt(const char* info, const int lower_bound, const int upper_bound);

/*
 * Demande un string à l'utilisateur. Continue de demander tant que la lecture n'a pas pu
 * se faire ou tant que le string entré est trop court.
 */
void askString(char* info, int minLength, char nomFichier[FILENAME_MAX + 1]);

Image diamond(int height, int width, int d);

char pixelToSymbol(Pixel pixel);

Pixel symboleToPixel(char ch);

void displayConvert(FILE* flot, Image img, bool convert);

void display(FILE* flot, Image img);

int write_to_file_convert(char* nom, Image img, bool convert);

int write_to_file(char* nom, Image img);

Image read_from_file_convert(char* nom, bool convert);

Image read_from_file(char* nom);

Image filter(Image img, Image masque);

bool imageCorrect(Image img);

int cycle(int n, int max);

/*
 * Retire du flot donné tous les caractères résiduels jusqu'au prochain retour à la ligne
 */
void cleanBuffer(FILE* f);


int main(){
    /*int
        width = askInt("la largeur de l'image", 1, MAX_IMAGE_WIDTH),
        height = askInt("la hauteur de l'image", 1, MAX_IMAGE_HEIGHT),
        diagonale = askInt("la taille de la diagonale", 0, min(width, height));
    Image img = diamond(height, width, diagonale);
    display(stdout, img);

    printf("\n\nNous allons maintenant écrire l'image dans un fichier et l'afficher à nouveau depuis ce même fichier.");
    char nomFichier[FILENAME_MAX + 1];
    askString("un nom de fichier", 3, nomFichier);
    write_to_file(nomFichier, img);
    Image imgLue = read_from_file(nomFichier);
    display(stdout, imgLue);

    Image masque = {
        .w = 3, .h = 3,
        .pixels = {
            {-2., -2., -2.},
            {0., 0., 0.},
            {2., 2., 2.}
        }
    };

    Image filteredImage = filter(img, masque);
    printf("\n\nNous allons maintenant filtrer l'image. L'image filtrée: \n");
    display(stdout, filteredImage);
    */
    display(stdout, read_from_file("test01.txt"));
    return 0;
}

int min(int n1, int n2){
    if(n1 < n2){
        return n1;
    }
    else{
        return n2;
    }
}

int getChar(FILE* flot, const char* info, const char allowed[], size_t const taille, char* toUpdate){
    char toReturn = ' ';
    bool notOk = true;


    while (!feof(stdin) && !ferror(stdin) && ( (toReturn = getc(flot)) == '\n' || toReturn == ' ') );
    for(size_t i = 0; i < taille; ++i){
        if(allowed[i] == toReturn){
            notOk = false;
            break;
        }
    }

    if (notOk) {
        fprintf(stderr, "Erreur lors de la lecture de: %s. Valeur non-conforme aux caractères attendus! Obtenu: %c\n", info, toReturn);
        return 2;
    }
    else{
        *toUpdate = toReturn;
        return 0;
    }
}

int getInt(FILE* flot, const char* info, const int lower_bound, const int upper_bound, int* toUpdate){
    int read = 0, toReturn = 0;
    bool notOk = true;

    read = fscanf(flot, "%d", &toReturn);

    if (read != 1) {
        fprintf(stderr, "Erreur lors de la lecture de: %s. Valeur non-entière détectée !", info);
        return 1;
    }
    else{
        notOk = toReturn < lower_bound || toReturn > upper_bound;
        if(notOk){
            fprintf(stderr, "Erreur lors de la lecture de %s. Valeur %d incorrecte. Contrainte: entre %d et %d.",
               info, toReturn, lower_bound, upper_bound);
            return 2;
        }
        else{
            *toUpdate = toReturn;
            return 0;
        }
    }
}

/*
 * Extrait une information réelle du flot donné et met à jour la variable transmise par son pointeur.
 * Retourne 0 si l'extraction s'est déroulée correctement, 1 si la lecture n'a pas pu se faire,
 * ou 2 si la lecture s'est faite mais que la valeur n'est pas comprise dans l'intervalle désiré.
 */
int getDouble(FILE* flot, const char* info, const double lower_bound, const double upper_bound, double* toUpdate){
    int read = 0;
    double toReturn = 0.;
    bool notOk = true;

    read = fscanf(flot, "%lf", &toReturn);

    if (read != 1) {
        fprintf(stderr, "Erreur lors de la lecture de %s. Valeur ne correspondant pas à un réel détectée !", info);
        return 1;
    }
    else{
        notOk = toReturn < lower_bound || toReturn > upper_bound;
        if(notOk){
            fprintf(stderr, "Erreur lors de la lecture de %s. Valeur %d incorrecte. Contrainte: entre %d et %d.",
               info, toReturn, lower_bound, upper_bound);
            return 2;
        }
        else{
            *toUpdate = toReturn;
            return 0;
        }
    }
}

int askInt(const char* info, const int lower_bound, const int upper_bound){
    int toReturn = 0, read = 0;

    do{
        printf("Entrez %s: ", info); fflush(stdout);
        read = getInt(stdin, info, lower_bound, upper_bound, &toReturn);

        if(read != 0){
            printf(" Veuillez réessayer.\n");
            while (!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
        }
    } while(read != 0);

    return toReturn;
}

/*
 * Demande un string à l'utilisateur. Continue de demander tant que la lecture n'a pas pu
 * se faire ou tant que le string entré est trop court.
 */
void askString(char* info, int minLength, char nomFichier[FILENAME_MAX + 1]){

    do{
        cleanBuffer(stdin);
        printf("\nEntrez %s: ", info);fflush(stdout);
        fgets(nomFichier, FILENAME_MAX + 1, stdin);

        nomFichier[strlen(nomFichier) - 1] = '\0';
        if(strlen(nomFichier) < minLength){
            printf(" -> ERREUR: nom de fichier trop court. Veuillez réessayer.");
        }
    } while(strlen(nomFichier) < minLength);
}

Image diamond(int height, int width, int d){
    #define INSIDE 1.
    #define OUTSIDE 0.

    // Make sure width and height are odd
    width |= 1;
    height |= 1;
    Image img = {.w = width, .h = height};

    if(!imageCorrect(img) || d < 0){
        fprintf(stderr,
                "Erreur: impossible de générer le losange. Fonction lancée avec un/des paramètre(s) non-valide(s)!\n");
        /*fprintf(stderr,
                "w: %d, h: %d, d: %d \n", img.w, img.h, d);*/
        img.w = 0;
    }
    else{

        const int xc = width / 2 + 1, yc = height / 2 + 1;
        //printf("Milieu: xc = %d , yc = %d \n", xc, yc);
        int x1 = 0;//, x2 = 1;
        Pixel pixel = 0.;


        for(int yp = 1; yp <= yc; ++yp){
            x1 = xc - (yp - yc + d / 2);
            //x2 = xc + (yp - yc + d / 2);
            //printf("x1 = %d , x2 = %d \n", x1, x2);
            for(int x = 1; x <= xc; ++x){
                if(x < x1){
                    pixel = OUTSIDE;
                }
                else{
                    pixel = INSIDE;
                }
                img.pixels[yp - 1][x - 1] = pixel;
                img.pixels[yp - 1][width - x] = pixel;
                img.pixels[height - yp][x - 1] = pixel;
                img.pixels[height - yp][width - x] = pixel;
            }
        }
    }

    return img;
}

char pixelToSymbol(Pixel pixel){
    if(pixel == 0.){
        return SYMBOLE_ZERO;
    }
    else if(pixel == 1.){
        return SYMBOLE_ONE;
    }
    else{
        return SYMBOLE_ELSE;
    }
}

Pixel symboleToPixel(char ch){
    if(ch == SYMBOLE_ONE){
        return 1.;
    }
    else if(ch == SYMBOLE_ZERO){
        return 0.;
    }
    else{
        return 2.;
    }
}

void displayConvert(FILE* flot, const Image img, const bool convert){
    if(!imageCorrect(img)){
        fprintf(stderr, "Erreur: Impossible d'afficher l'image dans le flot ! Dimensions de l'image non-valides.\n");
        fprintf(stderr, "w: %d, h: %d\n", img.w, img.h);
    }
    else{
        for(int h = 0; h < img.h; ++h){
            for(int w = 0; w < img.w; ++w){

                Pixel pixel = img.pixels[h][w];

                if(convert){
                    putc(pixelToSymbol(pixel), flot);
                }
                else{
                    fprintf(flot, "%f", pixel);
                }

                if(w != img.w - 1){
                    putc(' ', flot);
                }
            }

            if(h != img.h - 1){
                putc('\n', flot);
            }
        }
    }
}

void display(FILE* flot, Image img){
    displayConvert(flot, img, true);
}

int write_to_file_convert(char* nom, Image img, bool convert){
    FILE* f = fopen(nom, "w");

    if(f == NULL){
        return OPEN_ERROR;
    }
    else{
        fprintf(f, "%d\n%d\n", img.w, img.h);
        displayConvert(f, img, convert);

        fclose(f);
        return OPEN_OK;
    }
}

int write_to_file(char* nom, Image img){
    return write_to_file_convert(nom, img, true);
}

Image read_from_file_convert(char* nom, bool convert){
    FILE* f = fopen(nom, "r");
    Image img = {.w = 0, .h = 0};
    int read = 0;
    char ch = ' ';

    if(f == NULL){
        fprintf(stderr, "Impossible de lire depuis le fichier %s\n", nom);
    }
    else{
        read += getInt(f, "la largeur de l'image", 1, MAX_IMAGE_WIDTH, &(img.w));
        cleanBuffer(f);
        read += getInt(f, "la hauteur de l'image", 1, MAX_IMAGE_HEIGHT, &(img.h));
        cleanBuffer(f);

        for(int h = 0; h < img.h; ++h){
            for(int w = 0; w < img.w; ++w){
                if(read != 0){
                    img.h = 0;
                    img.w = 0;
                    fclose(f);
                    return img;
                }
                if(convert){
                    read += getChar(f, "pixel", SYMBOLS, 3, &ch);
                    img.pixels[h][w] = symboleToPixel(ch);
                }
                else{
                    read += getDouble(f, "pixel", 0., 2., &(img.pixels[h][w]) );
                }
            }
            cleanBuffer(f);
        }

        fclose(f);
    }

    return img;
}

Image read_from_file(char* nom){
    return read_from_file_convert(nom, true);
}

void cleanBuffer(FILE* f){
    while (!feof(f) && !ferror(f) && getc(f) != '\n');
}

Image filter(Image img, Image masque){
    Image newImage = {.h = img.h, .w = img.w};

    if(imageCorrect(img) && imageCorrect(masque)){
        int n = masque.h, indexImg1 = 0, indexImg2 = 0;
        Pixel pixel;

        for(int i = 0; i < img.h; ++i){
            for(int j = 0; j < img.w; ++j){
                pixel = 0.;
                for(int k = 0; k < n; ++k){
                    for(int l = 0; l < n; ++l){
                        indexImg1 = cycle(i + n/2 - k, img.h);
                        indexImg2 = cycle(j + n/2 - l, img.w);
                        pixel += img.pixels[indexImg1][indexImg2] * masque.pixels[k][l];
                    }
                }
                newImage.pixels[i][j] = pixel;
            }
        }
    }
    else{
        fprintf(stderr, "Erreur lors du filtrage de l'image ! Image ou masque non-valide.");
        img.w = 0;
    }

    return newImage;
}

bool imageCorrect(Image img){
    return img.h > 0 && img.h <= MAX_IMAGE_HEIGHT && img.w > 0 && img.w <= MAX_IMAGE_WIDTH;
}

int cycle(int n, int max){
    if(n >= 0){
        return n % max;
    }
    else{
        return max + n;
    }
}

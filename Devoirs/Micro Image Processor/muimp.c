#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_IMAGE_WIDTH 100
#define MAX_IMAGE_HEIGHT 100
#define MAX_INT 2147483647

typedef double Pixel;
typedef struct {
    size_t width;
    size_t height;
    Pixel pixels[MAX_IMAGE_WIDTH][MAX_IMAGE_HEIGHT];
} Image;

Image diamond(const int diag, int width, int height);
void display(FILE* output, const Image img);
int ask_int_with_bound(const char* s, const int low, const int up);
int write_to_file(const char* filename, const Image img);
Image read_from_file(const char* filename);
Image empty_image(void);
Image read_pixels_from_file(Image img, FILE* input);
bool assign_int(FILE* input, const int* dim, const char separator);
bool assign_int_with_bound(FILE* input, const int* dim, const char separator, const int low, const int up);
bool assign_pixel(FILE* input, Pixel* dim, const char separator);
Image filter(Image img, Image filter_mask);
double apply_mask(Image img, Image filter_mask, int img_pos_x, int img_pos_y);
int positive_modulo(int i, int n);

// return positive modulo of i % n
int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}
// filter an image using a filter
Image filter(Image img, Image filter_mask) {
    Image filtered_img;
    filtered_img.width = img.width;
    filtered_img.height = img.height;
    for( int y=0; y < img.height ; ++y) {
        for( int x=0; x < img.width ; ++x ) {
            filtered_img.pixels[x][y] = apply_mask(img, filter_mask, x, y);
        }
    }
    return filtered_img;
}
// apply mask on a given pixel from a given image using a given mask
double apply_mask(Image img, Image filter_mask, int img_pos_x, int img_pos_y) {
    double sum = 0.0;
    for( int y = 0 ; y < filter_mask.height ; ++y) {
        int j = positive_modulo(img_pos_y + y - filter_mask.height/2, img.height);
        for( int x = 0 ; x < filter_mask.width ; ++x ) {
            int i = positive_modulo(img_pos_x + x - filter_mask.width/2, img.width);
            sum += img.pixels[i][j] * filter_mask.pixels[x][y];
        }
    }
    return sum;
}
// return an empty image
Image empty_image(void) {
    Image img;
    img.height = 0;
    img.width = 0;
    return img;
}
// check if integers input is a correct image dimension and store it
bool assign_int_with_bound(FILE* input, const int* value, const char separator, const int low, const int up) {
    char sep;
    return ( fscanf(input, "%d%c", value, &sep) == 2 && sep == separator && *value >= low && *value<up);
}
// check if input is a correct image and store pixels
bool assign_pixel(FILE* input, Pixel* value, const char separator) {
    char c, sep;
    bool bool_scan = fscanf(input, "%c%c", &c, &sep) == 2;
    switch(c) {
    case '.' :
        *value = 0.0;
        break;
    case '+' :
        *value = 1.0;
        break;
    case '*' :
        *value = 2.0;
        break;
    default :
        return false;
    }
    return ( bool_scan && sep == separator );
}
// read pixels from a given file and an on-construction image
Image read_pixels_from_file(Image img, FILE* input) {
    for( int y=0; y < img.height ; ++y) {
        for( int x=0; x < img.width - 1; ++x ) {
            if(!assign_pixel(input, &img.pixels[x][y], ' ')) {
                return empty_image();
            }
        }
        if(!assign_pixel(input, &img.pixels[img.width-1][y], '\n')) {
            return empty_image();
        }
    }
    return img;
}
// read a file given its filename and return the image inside
Image read_from_file(const char* filename) {
    FILE* input = NULL;
    input = fopen(filename, "r");
    if(input == NULL) {
        return empty_image();
    } else {
        Image img;
        if( !assign_int_with_bound(input, &img.width, '\n', 1, MAX_IMAGE_WIDTH) ||
                !assign_int_with_bound(input, &img.height, '\n', 1, MAX_IMAGE_HEIGHT)
          ) {
            return empty_image();
        } else {
            return read_pixels_from_file(img, input);
        }
    }
}
// create a file given its filename and write a given image onto it
int write_to_file(const char* filename, const Image img) {
    FILE* output = NULL;
    output = fopen(filename, "w");
    if (output == NULL) {
        return 1;
    } else {
        fprintf(output, "%d\n%d\n", img.width, img.height);
        display(output, img);
        fclose(output);
        return 0;
    }
}
// ask user an integer between low inclusive and up exclusive
int ask_int_with_bound(const char* s, const int low, const int up) {
    int value;
    char term;
    do {
        printf("Please enter %s element of [%d, %d[ (only integers followed by a 'return line' are allowed) :\n", s, low, up);
        fflush(stdin);
    } while ( scanf("%d%c", &value, &term) != 2 || term != '\n' || value < low || value >= up);
    return value;
}
// display a given image to a given FILE*
void display(FILE* output, const Image img) {
    for( int y=0; y < img.height ; ++y) {
        for( int x=0; x < img.width; ++x ) {
            double pixel = img.pixels[x][y];
            if(pixel == 0.0) {
                fprintf(output, ".");
            } else if (pixel == 1.0) {
                fprintf(output, "+");
            } else {
                fprintf(output, "*");
            }
            if(x < img.width-1) {
                fprintf(output, " ");
            }
        }
        fprintf(output, "\n");
    }
}
// create a diamond image given the diagonal, width and height
Image diamond(const int diag, int width, int height) {
    width  |= 1;
    height |= 1;
    int centre_x = width/2,
        centre_y = height/2;
    Image img;

    img.height = height;
    img.width = width;
    for( int y=0; y < centre_y + 1 ; ++y) {
        for( int x=0; x < width; ++x ) {
            int edge1 = centre_x - (y - centre_y + diag/2),
                edge2 = centre_x + (y - centre_y + diag/2);
            if( x < edge1 || x > edge2 ) {
                img.pixels[x][y] = 0.0;
                img.pixels[x][height-y-1] = 0.0;
            } else {
                img.pixels[x][y] = 1.0;
                img.pixels[x][height-y-1] = 1.0;
            }
        }
    }
    return img;
}

int main(void) {
    //display(stdout, read_from_file("test01.txt"));
    // ask values to create a diamond image
    const int diag = ask_int_with_bound("diagonal", 0, MAX_INT);
    const int width = ask_int_with_bound("width", 0, MAX_IMAGE_WIDTH);
    const int height = ask_int_with_bound("height", 0, MAX_IMAGE_HEIGHT);
    // creation and display of the diamond image
    const Image img = diamond(diag, width, height);
    display(stdout, img);
    // asking filename
    char filename[FILENAME_MAX+1] = "";
    {
        int len;
        do {
            printf("Enter filename to write on this file :\n");
            fgets(filename, FILENAME_MAX+1, stdin);
            len = strlen(filename) - 1;
            if ((len >= 0) && (filename[len] == '\n'))
                filename[len] = '\0';
        } while ( (len < 1) && !feof(stdin) && !ferror(stdin) );
    }
    // writing image onto file with given filename
    write_to_file(filename, img);
    // reading image from file previously created and display it
    const Image img2 = read_from_file(filename);
    display(stdout, img2);
    // initialization of the mask to use it to filter images
    Image filter_mask = {3, 3, { {-2.0, 0.0, 2.0}, {-2.0, 0.0, 2.0}, {-2.0, 0.0, 2.0} } };
    // creation and display of a filtered image using the mask
    Image filtered_img = filter(img, filter_mask);
    display(stdout, filtered_img);
    return 0;
}

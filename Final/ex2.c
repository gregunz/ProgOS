#include <stdio.h>
#include <stdlib.h> //malloc

typedef enum {BLACK, WHITE, RED, GREEN} Color;

typedef struct {
    size_t x;
    size_t y;
} Point;

typedef struct Shape Shape;

typedef void(*draw_f)(const Shape* s);

struct Shape {
    Point p;
    size_t height;
    size_t width;
    Color c;
    draw_f f;
};

typedef struct {
    Shape** shapes;
    size_t n;
} Picture;

void add_shape(Picture* pic, Point top_left,
        size_t height, size_t width, Color c, draw_f f)
{
    Shape* new = malloc(sizeof(Shape));
    new->p = top_left;
    new->height = height;
    new->width = width;
    new->c = c;
    new->f = f;
    for (size_t i = 0; i < pic->n; ++i) {
        if (pic->shapes[i] == NULL) {
            pic->shapes[i] = new;
            pic->n += 1;
            return;
        }
    }
    Shape** result = realloc(pic->shapes, sizeof(Shape*) * (pic->n + 1));
    result[pic->n] = new;
    pic->n += 1;
    pic->shapes = result;
}

void draw_all(Picture* pic)
{
    for (size_t i = 0; i < pic->n; ++i) {
        (*pic->shapes[i]->f)(pic->shapes[i]);
    }
}

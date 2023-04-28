#include <SDL2/SDL.h>

typedef struct coord
{
    char x;
    int y;
} Coord;

SDL_Rect coord_to_rect(Coord n)
{
    int ix;
    if (n.x == 'a') ix = 1;
    else if (n.x == 'b') ix = 2;
    else if (n.x == 'c') ix = 3;
    else if (n.x == 'd') ix = 4;
    else if (n.x == 'e') ix = 5;
    else if (n.x == 'f') ix = 6;
    else if (n.x == 'g') ix = 7;
    else if (n.x == 'h') ix = 8;

    return (SDL_Rect) { (ix - 1) * 100, (8 - n.y) * 100, 100, 100 };
}

Coord pos_to_coord(int x, int y)
{
    char ix; 
    int iy;

    if (x >= 0 && x < 100) ix = 'a';
    else if (x >= 100 && x < 200) ix = 'b';
    else if (x >= 200 && x < 300) ix = 'c';
    else if (x >= 300 && x < 400) ix = 'd';
    else if (x >= 400 && x < 500) ix = 'e';
    else if (x >= 500 && x < 600) ix = 'f';
    else if (x >= 600 && x < 700) ix = 'g';
    else if (x >= 700 && x <= 800) ix = 'h';
    
    if (y >= 0 && y < 100) iy = 8;
    else if (y >= 100 && y < 200) iy = 7;
    else if (y >= 200 && y < 300) iy = 6;
    else if (y >= 300 && y < 400) iy = 5;
    else if (y >= 400 && y < 500) iy = 4;
    else if (y >= 500 && y < 600) iy = 3;
    else if (y >= 600 && y < 700) iy = 2;
    else if (y >= 700 && y <= 800) iy = 1;
    
    return (Coord) {ix, iy};
}

Coord rect_to_coord(SDL_Rect rect)
{
    return pos_to_coord(rect.x, rect.y);
}


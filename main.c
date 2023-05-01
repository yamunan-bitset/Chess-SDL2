// #define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "coord.h"

#define W 800
#define H 800
/*TODO: implement preprocessors*/
/*TODO: implement stockfish*/

struct piece
{
    SDL_Texture *tex;
    Coord coord; 
    bool white;
} p[32];

void Board(SDL_Renderer *render)
{
    int iters = 0;
    for (int y = 0; y < 8; y++)
    {       
        for (int x = 0; x < 8; x++) 
        {           
                SDL_Rect rect;
                rect.x = x * 100;
                rect.y = y * 100;
                rect.w = 100;
                rect.h = 100;
                if (iters % 2 == 1) 
                    SDL_SetRenderDrawColor(render, 0xA9, 0x7A, 0x65, 255);
                else SDL_SetRenderDrawColor(render, 0xF1, 0xD9, 0xC0, 255);
                SDL_RenderFillRect(render, &rect);
                iters++;
        }
        iters++;
    }
}

void RenderPieces(SDL_Renderer *render)
{
	for (int i = 0; i < 32; i++)
        {
            SDL_Rect tmp = coord_to_rect(p[i].coord);
            SDL_RenderCopy(render, p[i].tex, NULL, &tmp);
        }
	SDL_RenderPresent(render);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    
    SDL_Renderer *render = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface *surf = SDL_GetWindowSurface(window);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    Board(render);

    SDL_Surface *surfs[32];
    surfs[0] = surfs[7] = IMG_Load("data/assets/b_rook.png");
    surfs[1] = surfs[6] = IMG_Load("data/assets/b_knight.png");
    surfs[2] = surfs[5] = IMG_Load("data/assets/b_bishop.png");
    surfs[3] = IMG_Load("data/assets/b_queen.png");
    surfs[4] = IMG_Load("data/assets/b_king.png");
    for (int i = 8; i < 16; i++)
        surfs[i] = IMG_Load("data/assets/b_pawn.png");
    for (int i = 16; i < 24; i++)
        surfs[i] = IMG_Load("data/assets/w_pawn.png");
    surfs[24] = surfs[31] = IMG_Load("data/assets/w_rook.png");
    surfs[25] = surfs[30] = IMG_Load("data/assets/w_knight.png");
    surfs[26] = surfs[29] = IMG_Load("data/assets/w_bishop.png");
    surfs[27] = IMG_Load("data/assets/w_queen.png");
    surfs[28] = IMG_Load("data/assets/w_king.png");

    SDL_Texture *textures[32];
    for (int i = 0; i < 32; i++)
    {
        textures[i] = SDL_CreateTextureFromSurface(render, surfs[i]);
        p[i].tex = textures[i];
    }

    char letters[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
    for (int i = 0; i < 8; i++)
    {
        p[i].coord = (Coord) {letters[i], 8};
        p[i].white = false;
    }
    for (int i = 8; i < 16; i++)
    {
        p[i].coord = (Coord) {letters[i % 8], 7};
        p[i].white = false;
    }
    for (int i = 16; i < 24; i++)
    {
        p[i].coord = (Coord) {letters[i % 8], 2};
        p[i].white = true;
    }
    for (int i = 24; i < 32; i++)
    {
        p[i].coord = (Coord) {letters[i % 8], 1};
        p[i].white = true;
    }

    Board(render);
    RenderPieces(render);
    
    bool quit = false;
    SDL_Event event;
    int mouseX, mouseY, selected = 32;
    bool white_turn = true;
	bool render_pieces = true;
    while (!quit)
    {
      SDL_WaitEvent(&event);
      	switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouseX, &mouseY);
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        if (selected < 32)
                        {
                            printf("%c%i\n", pos_to_coord(mouseX, mouseY).x, pos_to_coord(mouseX, mouseY).y);
                            p[selected].coord.x = pos_to_coord(mouseX, mouseY).x;
                            p[selected].coord.y = pos_to_coord(mouseX, mouseY).y;
                            for (int i = 0; i < 32; i++)
                                if ((white_turn && !p[i].white) || (!white_turn && p[i].white))
                                    if (p[i].coord.x == pos_to_coord(mouseX, mouseY).x
                                        && p[i].coord.y == pos_to_coord(mouseX, mouseY).y)
                                        p[i].tex = NULL;
                            selected = 32;
			    render_pieces = true;
			    white_turn = !white_turn;
                        }
                        else
                        {
                            for (int i = 0; i < 32; i++)
                            {
                                if ((white_turn && p[i].white) || (!white_turn && !p[i].white))
                                    if (p[i].coord.x == pos_to_coord(mouseX, mouseY).x && p[i].coord.y == pos_to_coord(mouseX, mouseY).y)
                                    {
                                        printf("%c%i-", p[i].coord.x, p[i].coord.y);
                                        selected = i;
                                        SDL_Rect rect = coord_to_rect(p[i].coord);
                                        SDL_SetRenderDrawColor(render, 0xE5, 0xDE, 0x00, 255);
                                        SDL_RenderFillRect(render, &rect);
                                        SDL_RenderPresent(render);
                                    }
				
                            }
                        }
                        break;
                    case SDL_BUTTON_RIGHT:
                        SDL_SetRenderDrawColor(render, 0xC1, 0x19, 0x20, 255);
                        SDL_Rect rect = coord_to_rect(pos_to_coord(mouseX, mouseY));
                        SDL_RenderFillRect(render, &rect);
                        SDL_RenderPresent(render);
                        break;
                }
                break;
        }
		if (render_pieces)
		  {
		    SDL_RenderClear(render);
		    Board(render);
		    RenderPieces(render);
		    render_pieces = false;
		  }
	}       
    SDL_Quit();
    return 0;
}

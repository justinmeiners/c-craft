
#include "game.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <OpenGL/gl.h>

int _simTime = 0;
int _width;
int _height;
float _mouseX;
float _mouseY;

float _lastX = 0.0f;
float _lastY = 0.0f;

int _up = 0;
int _down = 0;

int _left = 0;
int _right = 0;

int _space = 0;
int _click = 0;
int _rightClick = 0;

int _numbers[9] =
{
    0,0,0,
    0,0,0,
    0,0,0,

};

Game_t game;

void keyDown(unsigned int keycode)
{
    int st = 1;
    switch (keycode)
    {
        case SDL_SCANCODE_W:
            _up = st;
            break;
        case SDL_SCANCODE_S:
            _down = st;
            break;
        case SDL_SCANCODE_A:
            _left = st;
            break;
        case SDL_SCANCODE_D:
            _right = st;
            break;
        case SDL_SCANCODE_SPACE:
            _space = st;
            break;
        case SDL_SCANCODE_1:
            _numbers[0] = st;
            break;
        case SDL_SCANCODE_2:
            _numbers[1] = st;
            break;
        case SDL_SCANCODE_3:
            _numbers[2] = st;
            break;
        case SDL_SCANCODE_4:
            _numbers[3] = st;
            break;
        case SDL_SCANCODE_5:
            _numbers[4] = st;
            break;
        default:
            break;
    }
}

void keyUp(unsigned int keycode)
{
    int st = 0;
    switch (keycode)
    {
        case SDL_SCANCODE_W:
            _up = st;
            break;
        case SDL_SCANCODE_S:
            _down = st;
            break;
        case SDL_SCANCODE_A:
            _left = st;
            break;
        case SDL_SCANCODE_D:
            _right = st;
            break;
        case SDL_SCANCODE_SPACE:
            _space = st;
            break;
        case SDL_SCANCODE_1:
            _numbers[0] = st;
            break;
        case SDL_SCANCODE_2:
            _numbers[1] = st;
            break;
        case SDL_SCANCODE_3:
            _numbers[2] = st;
            break;
        case SDL_SCANCODE_4:
            _numbers[3] = st;
            break;
        case SDL_SCANCODE_5:
            _numbers[4] = st;
            break;
        case SDL_SCANCODE_E:
            Game_ToggleInventory(&game);
            break;
        default:
            break;
    }
}

void mouseMove(int mx, int my)
{
    _mouseX = (mx / (float)_width) * 2.0f - 1.0f;
    _mouseY = (1.0 - my / (float)_height) * 2.0f - 1.0f;
}


void reshape(int width, int height)
{
    _width = width;
    _height = height;
    glViewport(0, 0, _width, _height);
}


int main(int argc, char * argv[])
{    
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "SDL init failed\n");
        return 1;
    }
    
    _width = 1024;
    _height = 768;

    SDL_Window* window = SDL_CreateWindow("ccraft", 0, 0, _width, _height, SDL_WINDOW_OPENGL);
    if (!window)
    {
        fprintf(stderr, "SDL create window error: %s\n", SDL_GetError());
        SDL_Quit();
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fprintf(stderr, "create context error\n");
        SDL_Quit();
    }

    reshape(_width, _height);
   
    SDL_GL_SetSwapInterval(1);
    
    Game_Init(&game);

    
    for (;;)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            int x;
            int y;
            switch (e.type)
            {
                case SDL_QUIT:
                    Game_Quit(&game);
                    SDL_Quit();
                    return 0;
                case SDL_KEYDOWN:
                    keyDown(e.key.keysym.scancode);
                    break;
                case SDL_KEYUP:
                    keyUp(e.key.keysym.scancode);
                    break;
                case SDL_MOUSEMOTION:
                    if (game.state.mode == MODE_GAME)
                    {
                        SDL_GetRelativeMouseState(&x, &y);
                        Game_MoveCamera(&game, (x / (float)_width),
                                        (y / (float)_height));
                    }
                    else
                    {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        Game_SetCursor(&game, x / (float)_width, y / (float)_height);
                    }

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) _click = 1;
                    if (e.button.button == SDL_BUTTON_RIGHT) _rightClick = 1;

                    break;
                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT) _click = 0;
                    if (e.button.button == SDL_BUTTON_RIGHT) _rightClick = 0;

                    break;
            }
        }
        
        for (int i = 0; i < 5; i ++)
        {
            if (_numbers[i])
            {
                Game_SelectInventoryItem(&game, i);
            }
        }
        
        game.forward = _up - _down;
        game.side = _right - _left;
        game.jumping = _space;
        game.digging = _click;
        game.placing = _rightClick;
    
        
        Game_Update(&game);
        
        SDL_SetRelativeMouseMode(game.state.mode == MODE_GAME);
        SDL_GL_SwapWindow(window);
        Game_Render(&game);
        SDL_Delay(17);
    }

    return 0;
}


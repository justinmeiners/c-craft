
#ifndef ccraft_renderer_h
#define ccraft_renderer_h

#include "vec_math.h"
#include "cam.h"
#include "world.h"
#include "inventory.h"
#include "state.h"

#include <OpenGL/gl.h>


typedef struct
{
    Mat4_t mvpMat;
    GLuint blockAtlas;
    GLuint itemAtlas;
} Renderer_t;

extern void Renderer_Init(Renderer_t* renderer);

extern void Renderer_RenderWorld(Renderer_t* renderer,
                                 Cam_t* cam,
                                 const World_t* world,
                                 Inventory_t* inventory,
                                 Inventory_t* belt,
                                 const State_t* state);


#endif

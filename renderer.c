
#include "renderer.h"
#include "targa.h"

static GLuint Renderer_Upload(Renderer_t* renderer, short w, short h, int rgba, const GLubyte* data)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    if (rgba)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return tex;
}

void Renderer_Init(Renderer_t* renderer)
{
    tga_image image;
    tga_read(&image, "textures/block_textures.tga");
    tga_swap_red_blue(&image);
    
    renderer->blockAtlas = Renderer_Upload(renderer, image.width, image.height, 0, image.image_data);
    
    tga_image image2;
    tga_read(&image2, "textures/item_textures.tga");
    tga_swap_red_blue(&image2);
    
    renderer->itemAtlas = Renderer_Upload(renderer, image2.width, image2.height, 1, image2.image_data);
    
    
    glEnable(GL_DEPTH_TEST);
    glPointSize(20.0f);
}


#define ITEM_ATLAS_ROWS 8
#define ITEM_ATLAS_SIZE 0.125

static Vec2_t ItemAtlas_UVForTex(int texID)
{
    Vec2_t uv;
    uv.x = (texID % ITEM_ATLAS_ROWS) * ITEM_ATLAS_SIZE;
    uv.y = (texID / ITEM_ATLAS_ROWS) * ITEM_ATLAS_SIZE;
    return uv;
}

static int ItemAtlas_TexForItem(int itemType)
{
    
    switch (itemType)
    {
        case ITEM_DIRT:
            return 1;
        case ITEM_STONE:
            return 2;
        case ITEM_SHOVEL:
            return 8;
        case ITEM_TURF:
            return 0;
        case ITEM_GIFT:
            return 3;
    }
    
    return 0;
}

static void _Renderer_DrawChunks(Renderer_t* renderer,
                                 Cam_t* cam,
                                 const World_t* world)
{
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, renderer->blockAtlas);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    int i;
    for (i = 0; i < world->chunkCount; ++i)
    {
        const Chunk_t* chunk = &world->chunks[i];
        
        if (!Cam_SphereVisible(cam, chunk->boundingSphere)) continue;
        
        glPushMatrix();
        glTranslatef(chunk->worldPosition.x, chunk->worldPosition.y, chunk->worldPosition.z);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(3, GL_SHORT, sizeof(Vert_t), &chunk->cache[0].verts[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vert_t),  &chunk->cache[0].verts[0].u);
        
        glDrawArrays(GL_QUADS, 0, chunk->cacheSize * 4);
        
        glPopMatrix();
    }
}

static void _Renderer_DrawEntities(Renderer_t* renderer,
                                   Cam_t* cam,
                                   const World_t* world)
{
    
    glDisable(GL_TEXTURE_2D);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    
    int i;
    for (i = 0; i < MAX_ENTITIES; ++i)
    {
        const Entity_t* entity = world->entities + i;
        
        if (entity->entityID != -1)
        {
            glBegin(GL_POINTS);
            glVertex3f(entity->position.x, entity->position.y, entity->position.z);
            glEnd();
        }
    }
}

static void _Renderer_DrawInventory(Renderer_t* renderer,
                                    Inventory_t* inventory,
                                    const State_t* state,
                                    float ox,
                                    float oy)
{
    
    glDisable(GL_DEPTH_TEST);
    
    int x,y;
    
    float tSize = 32.0f;
    float tSpacing = 2.0f;
    
    glBegin(GL_QUADS);
    
    for (x = 0; x < inventory->width; ++x)
    {
        for (y = 0; y < inventory->height; ++y)
        {
            float tx = ox + (float)x * (tSize + tSpacing);
            float ty = oy + (float)y * (tSize + tSpacing);
            
            if (state->mode == MODE_INVENTORY &&
                state->cursor.x > tx &&
                state->cursor.y > ty &&
                state->cursor.x < tx + tSize &&
                state->cursor.y < ty + tSize)
            {
                inventory->selectedItem = x + y * inventory->width;
            }
            
            if (inventory->selectedItem == x + y * inventory->width)
            {
                glColor3f(1.0f, 1.0f, 0.0f);
                glVertex2f(tx - 1, ty - 1);
                glVertex2f(tx + tSize + 1, ty - 1);
                glVertex2f(tx + tSize + 1, ty + tSize + 1);
                glVertex2f(tx - 1, ty + tSize + 1);
            }
            glColor3f(0.25f, 0.25f, 0.25f);
            
            
            glVertex2f(tx, ty);
            glVertex2f(tx + tSize, ty);
            glVertex2f(tx + tSize, ty + tSize);
            glVertex2f(tx, ty + tSize);
        }
    }
    
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, renderer->itemAtlas);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    
    for (x = 0; x < inventory->width; ++x)
    {
        for (y = 0; y < inventory->height; ++y)
        {
            if (inventory->items[x + y * inventory->width].type == ITEM_NONE) continue;
            
            float tx = ox + (float)x * (tSize + tSpacing);
            float ty = oy + (float)y * (tSize + tSpacing);
            
            Vec2_t uv = ItemAtlas_UVForTex(ItemAtlas_TexForItem(inventory->items[x + y * inventory->width].type));
            
            glTexCoord2f(uv.x, uv.y);
            glVertex2f(tx, ty);
            glTexCoord2f(uv.x + ITEM_ATLAS_SIZE, uv.y);
            glVertex2f(tx + tSize, ty);
            glTexCoord2f(uv.x + ITEM_ATLAS_SIZE, uv.y + ITEM_ATLAS_SIZE);
            glVertex2f(tx + tSize, ty + tSize);
            glTexCoord2f(uv.x, uv.y + ITEM_ATLAS_SIZE);
            glVertex2f(tx, ty + tSize);
        }
    }
    
    glEnd();
}

void Renderer_RenderWorld(Renderer_t* renderer,
                          Cam_t* cam,
                          const World_t* world,
                          Inventory_t* inventory,
                          Inventory_t* belt,
                          const State_t* state)
{
    glClearColor(.620f, .807f, .980f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(Cam_ProjectionMat(cam)->m);
    
    
    glEnable(GL_DEPTH_TEST);
    
    Mat4_Mult(Cam_ProjectionMat(cam), Cam_ViewMat(cam), &renderer->mvpMat);
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(Cam_ViewMat(cam)->m);
    
    _Renderer_DrawChunks(renderer, cam, world);
    _Renderer_DrawEntities(renderer, cam, world);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (state->mode == MODE_INVENTORY)
    {
        _Renderer_DrawInventory(renderer, inventory, state, 16.0f, 550.0f);
    }
    
    _Renderer_DrawInventory(renderer, belt, state, 16.0f, 700.0f);
    
    /*
     glBegin(GL_TRIANGLES);
     
     glVertex3f(0.5f, -0.25f, 0.0f);
     glVertex3f(2.0f, -2.0f, 0.0f);
     glVertex3f(0.5f, 0.1f, 0.0f);
     
     glEnd();
     */
}


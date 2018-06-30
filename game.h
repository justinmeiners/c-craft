
#ifndef ccraft_game_h
#define ccraft_game_h

#include "topology.h"
#include "renderer.h"
#include "cam.h"
#include "inventory.h"
#include "state.h"

typedef struct
{
    Vec3_t position;
    float pitch;
    float yaw;
    Vec3_t velocity;
    int onGround;
    
    float radius;
    float height;
    
    int groundBlockType;
    
    Inventory_t belt;
    Inventory_t pack;
} Player_t;

extern void Player_Init(Player_t* player);
extern void Player_Pickup(Player_t* player, int itemType, int qty);

typedef struct
{
    Renderer_t renderer;
    Cam_t cam;
    State_t state;
    
    Player_t player;
    Vec3_t gravity;
    Vec3_t entityGravity;

    World_t world;
    
    int forward;
    int side;
    int jumping;
    int digging;
    int placing;
    
    Block_t* selectedBlock;
    
    int beltIndex;
    
    int loadDist;
    
    int cx;
    int cy;
    int cz;
    
} Game_t;

extern void Game_Init(Game_t* game);

extern void Game_Render(Game_t* game);
extern void Game_Update(Game_t* game);

extern void Game_MoveCamera(Game_t* game, float deltaX, float deltaY);
extern void Game_SetCursor(Game_t* game, float x, float y);

extern void Game_SelectInventoryItem(Game_t* game, int item);

extern void Game_ToggleInventory(Game_t* game);

extern void Game_Quit(Game_t* game);

#endif

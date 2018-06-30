#include "game.h"
#include <stdlib.h>

static inline float clampf(float t, float a, float b)
{
    return t > a ? (t < b ? t : b) : a;
}

void Player_Init(Player_t* player)
{
    player->position = Vec3_Create(1.0f, 1.0f, 10.0f);
    player->pitch = 90.0f;
    player->yaw = 0.0f;
    player->velocity = Vec3_Zero();
    player->onGround = 0;
    
    player->radius = 0.25f;
    player->height = 1.6f;
    
    player->groundBlockType = BLOCK_AIR;
    
    Inventory_Init(&player->belt, 8, 1);
    Inventory_Init(&player->pack, 10, 4);
    
    Inventory_AddItem(&player->pack, ITEM_SHOVEL, 1);
    Inventory_AddItem(&player->pack, ITEM_GIFT, 10);
}

void Player_Pickup(Player_t* player, int itemType, int qty)
{
    if (!Inventory_AddItem(&player->belt, itemType, qty))
    {
        Inventory_AddItem(&player->pack, itemType, qty);
    }
}

static void Game_UpdatePlayer(Game_t* game, Player_t* player)
{
    float maxSpeed = 0.1f;
    
    if (player->groundBlockType == BLOCK_TRACK)
    {
        maxSpeed *= 1.8f;
    }
    else if (player->groundBlockType == BLOCK_MUD)
    {
        maxSpeed *= 0.5f;
    }
    
    if (fabs(player->velocity.x) < maxSpeed && fabs(player->velocity.y) < maxSpeed)
    {
        if (game->forward != 0)
        {
            float rads = (player->yaw * M_PI) / 180.0f;
            Vec3_t forwardVec = Vec3_Create(cosf(rads), sinf(rads), 0.0f);
            
            player->velocity = Vec3_Add(player->velocity, Vec3_Scale(forwardVec, game->forward * 0.05f));
        }
        
        if (game->side != 0)
        {
            float rads = (player->yaw * M_PI) / 180.0f;
            Vec3_t sideVec = Vec3_Create(cosf(rads - M_PI / 2), sinf(rads - M_PI / 2), 0.0f);
            player->velocity = Vec3_Add(player->velocity, Vec3_Scale(sideVec, game->side * 0.05f));
        }
    }
    
    if (game->jumping != 0 && player->onGround)
    {
        player->velocity.z = 0.15f;
    }
    
    
    //if (!player->onGround)
    {
        player->velocity = Vec3_Add(player->velocity, game->gravity);
    }
    
    int x = floorf(player->position.x);
    int y = floorf(player->position.y);
    int z = floorf(player->position.z);
    
    Vec3_t dest = Vec3_Add(player->position, player->velocity);
    
    
    int dx = floorf(dest.x);
    int dy = floorf(dest.y);
    int dz = floorf(dest.z);
    
    Block_t* block;
    
    //if (dx != x)
    {
        block = World_GetBlockAt(&game->world, dx, y, z);
        if (block && block->type != BLOCK_AIR)
        {
            if (block->type == BLOCK_BOUNCE_PAD)
            {
                player->velocity.x = -player->velocity.x * 0.95f;
            }
            else
            {
                player->velocity.x = 0.0f;
                player->position.x = clampf(player->position.x, x + 0.001f , x + 0.999f);
            }
        }
    }
    
    x = floorf(player->position.x);
    
    //if (dy != y)
    {
        block = World_GetBlockAt(&game->world, x, dy, z);
        if (block && block->type != BLOCK_AIR)
        {
            if (block->type == BLOCK_BOUNCE_PAD)
            {
                player->velocity.y = -player->velocity.y * 0.95f;
            }
            else
            {
                player->velocity.y = 0.0f;
                player->position.y = clampf(player->position.y, y + 0.001f, y + 0.999f);
            }
        }
    }
    
    y = floorf(player->position.y);
    
    //if (dz != z)
    {
        block = World_GetBlockAt(&game->world, x, y, dz);
        if (block && block->type != BLOCK_AIR)
        {
            if (block->type == BLOCK_BOUNCE_PAD)
            {
                player->velocity.z = -player->velocity.z * 0.95f;
            }
            else
            {
                player->velocity.z = 0.0f;
                player->position.z = clampf(player->position.z, z + 0.001f, z + 0.999f);
                
                /* block is below us, not above */
                if (dz < z)
                {
                    player->groundBlockType = block->type;
                    player->onGround = 1;
                }
            }
        }
        else
        {
            player->onGround = 0;
        }
    }

    
    if (player->groundBlockType == BLOCK_ICE)
    {
        player->velocity.x *= .97;
        player->velocity.y *= .97;
    }
    else
    {
        player->velocity.x *= .85;
        player->velocity.y *= .85;
    }

    player->position = Vec3_Add(player->position, player->velocity);
}


void Game_Init(Game_t* game)
{
    Renderer_Init(&game->renderer);
    Cam_Init(&game->cam);
    game->cam.near = 0.1f;
    game->cam.far = 100.0f;
    game->cam.fov = 70.0f;
    game->beltIndex = 0;
    
    game->cam.orientation = Vec3_Create(0.0f, 0.0f, 1.0f);
    
    game->state.mode = MODE_GAME;
    
    game->cx = -1;
    game->cy = -1;
    game->cz = -1;
    
    World_Init(&game->world);

    game->gravity = Vec3_Create(0.0f, 0.0f, -0.008f);
    game->entityGravity = Vec3_Create(0.0f, 0.0f, -0.006f);
    
    Player_Init(&game->player);
    
    game->loadDist = 2;
        
    game->selectedBlock = NULL;
}

void Game_Render(Game_t* game)
{
    Topologize_World(&game->cam, &game->world);
    Renderer_RenderWorld(&game->renderer, &game->cam, &game->world, &game->player.pack, &game->player.belt, &game->state);
}

static void _Game_UpdateCamera(Game_t* game)
{
    /* camera */
    game->cam.position = Vec3_Add(game->player.position, Vec3_Create(0.0f, 0.0f, 1.6f));
    
    Quat_t quat = Quat_FromEuler(game->player.pitch, game->player.yaw, 0.0f);
    
    game->cam.target = Vec3_Create(1.0f, 0.0f, 0.0f);
    
    Vec3_t targetDir = Quat_RotateVec3(quat, game->cam.target);
    game->cam.target = Vec3_Add(targetDir, game->cam.position);
    
    Cam_UpdateTransform(&game->cam, 1024, 768);
}

static void _Game_UpdateChunks(Game_t* game)
{
    int cx = floorf(game->player.position.x) / CHUNK_SIZE;
    int cy = floorf(game->player.position.y) / CHUNK_SIZE;
    int cz = floorf(game->player.position.z) / CHUNK_SIZE;
    
    if (cx != game->cx ||
        cy != game->cy ||
        cz != game->cz)
    {
        int x, y;
        for (x = -game->loadDist; x < game->loadDist; x ++)
        {
            for (y = -game->loadDist; y < game->loadDist; y ++)
            {
                if (cx + x >= 0 && cy + y >= 0)
                {
                    World_PrepareChunk(&game->world, cx + x, cy + y, 0);
                }
            }
        }
        
        /*
         int i;
         for (i = 0; i < game->world.chunkCount; i ++)
         {
         if (abs(game->world.chunks[i].x - cx) > game->unloadDist ||
         abs(game->world.chunks[i].y - cy) > game->unloadDist)
         {
         game->world.chunks[i].needsToUnload = 1;
         }
         }
         */
        
        game->cx = cx;
        game->cy = cy;
        game->cz = cz;
    }
}

static void _Game_UpdateEntities(Game_t* game)
{
    int i;
    for (i = 0; i < MAX_ENTITIES; i ++)
    {
        Entity_t* entity = game->world.entities + i;
        
        if (entity->entityID == -1) continue;
        
        int ex = floorf(entity->position.x);
        int ey = floorf(entity->position.y);
        int ez = floorf(entity->position.z);
        
        Block_t* eblock = World_GetBlockAt(&game->world, ex, ey, ez - 1);
        
        if (!eblock || eblock->type == BLOCK_AIR || entity->position.z - (float)ez > 0.25f)
        {
            entity->velocity = Vec3_Add(entity->velocity, game->entityGravity);
        }
        else
        {
            entity->velocity = Vec3_Zero();
        }
        
        entity->position = Vec3_Add(entity->position, entity->velocity);
        
        if (entity->entityID != -1)
        {
            if (Vec3_DistSq(game->player.position, entity->position) < (entity->size * entity->size + 0.25f))
            {
                Player_Pickup(&game->player, entity->pickupType, entity->qty);
                entity->entityID = -1;
            }
        }
    }
    
}

static void _Game_UpdateTools(Game_t* game)
{
    Quat_t quat = Quat_FromEuler(game->player.pitch, game->player.yaw, 0.0f);
    Vec3_t targetDir = Quat_RotateVec3(quat, Vec3_Create(1.0f, 0.0f, 0.0f));
    Vec3_t aim = Vec3_Add(game->cam.position, Vec3_Scale(targetDir, 2.0f));
    
    int tx = floorf(aim.x);
    int ty = floorf(aim.y);
    int tz = floorf(aim.z);
    
    Block_t* block = World_GetBlockAt(&game->world, tx, ty, tz);
    
    if (block != game->selectedBlock)
    {
        
        game->selectedBlock = block;
    }
    
    _Game_UpdateEntities(game);
    
    int invIndex = game->player.belt.selectedItem;
    Inventory_t* inv = &game->player.belt;
    
    if (inv->items[invIndex].type == ITEM_SHOVEL)
    {
        if (game->digging)
        {
            if (block && block->type == BLOCK_GRASS)
            {
                block->type = BLOCK_DIRT;
                
                Entity_t* entity =  World_SpawnEntity(&game->world, ENTITY_TURF);
                entity->pickupType = ITEM_TURF;
                entity->qty = 1;
                entity->size = 0.5f;
                entity->height = 0.5f;
                entity->position = Vec3_Create(tx + 0.5f, ty + 0.5f, tz + 1.5f);
                
                World_UpdateBlockAt(&game->world, tx, ty, tz);
            }
        }
    }
    else if (inv->items[invIndex].type == ITEM_TURF)
    {
        if (game->placing)
        {
            if (block && block->type == BLOCK_DIRT)
            {
                block->type = BLOCK_GRASS;
                Inventory_RemoveItem(inv, invIndex, 1);
                World_UpdateBlockAt(&game->world, tx, ty, tz);
            }
        }
    }
    else
    {
        /* digging */
        if (game->digging)
        {
            if (block && block->type != BLOCK_SOLID)
            {
                if (block->type == BLOCK_STONE || block->type == BLOCK_DIRT || block->type == BLOCK_GRASS || block->type == BLOCK_GIFT)
                {
                    int type;
                    int itemType;
                    
                    switch (block->type)
                    {
                        case BLOCK_STONE:
                            type = ENTITY_STONE;
                            itemType = ITEM_STONE;
                            break;
                        case BLOCK_DIRT:
                            type = ENTITY_DIRT;
                            itemType = ITEM_DIRT;
                            break;
                        case BLOCK_GRASS:
                            type = ENTITY_DIRT;
                            itemType = ITEM_DIRT;
                            break;
                        case BLOCK_GIFT:
                            type = ENTITY_GIFT;
                            itemType = ITEM_NONE + rand() % ITEM_COUNT;
                    }
                    
                    Entity_t* entity =  World_SpawnEntity(&game->world, ENTITY_STONE);
                    entity->pickupType = itemType;
                    entity->qty = 1;
                    entity->size = 0.5f;
                    entity->height = 0.5f;
                    entity->position = Vec3_Create(tx + 0.5f, ty + 0.5f, tz + 0.5f);
                }
                
                block->type = BLOCK_AIR;
                
                World_UpdateBlockAt(&game->world, tx, ty, tz);
            }
        }
        else if (game->placing)
        {
            if (block && block->type == BLOCK_AIR)
            {
                if (invIndex != -1 && inv->items[invIndex].type != ITEM_NONE)
                {
                    switch (inv->items[invIndex].type)
                    {
                        case ITEM_DIRT:
                            block->type = BLOCK_DIRT;
                            break;
                        case ITEM_STONE:
                            block->type = BLOCK_STONE;
                            break;
                        case ITEM_GIFT:
                            block->type = BLOCK_GIFT;
                            
                        default:
                            break;
                    }
                    
                    Inventory_RemoveItem(inv, invIndex, 1);
                }
                
                World_UpdateBlockAt(&game->world, tx, ty, tz);
            }
        }
    }
}

static void _Game_UpdateInventory(Game_t* game)
{
    if (game->digging)
    {
        int packIndex = game->player.pack.selectedItem;
        int beltIndex = game->player.belt.selectedItem;

        if (game->player.belt.items[beltIndex].type == ITEM_NONE)
        {
            game->player.belt.items[beltIndex].type = game->player.pack.items[packIndex].type;
            game->player.belt.items[beltIndex].qty++;
        }
        else
        {
            Inventory_AddItem(&game->player.belt, game->player.pack.items[packIndex].type, 1);
        }
        
        Inventory_RemoveItem(&game->player.pack, packIndex, 1);
    }
}

void Game_Update(Game_t* game)
{
    Game_UpdatePlayer(game, &game->player);
    _Game_UpdateChunks(game);
    _Game_UpdateCamera(game);
    
    if (game->state.mode == MODE_GAME)
    {
        _Game_UpdateTools(game);
    }
    else if (game->state.mode == MODE_INVENTORY)
    {
        _Game_UpdateInventory(game);
    }
}

void Game_MoveCamera(Game_t* game, float deltaX, float deltaY)
{
    game->player.yaw -= deltaX * 50.0f;
    game->player.pitch += deltaY * 50.0f;
    
    game->player.pitch = clampf(game->player.pitch, -89.0f, 89.0f);
    

    //printf("%f, %f", deltaX, deltaY);
}

void Game_SetCursor(Game_t* game, float x, float y)
{
    game->state.cursor.x = x * 1024.0f;
    game->state.cursor.y = (1.0f - y) * 768.0f;
}

void Game_SelectInventoryItem(Game_t* game, int item)
{
    game->player.belt.selectedItem = item;
    game->beltIndex = item;
}

void Game_ToggleInventory(Game_t* game)
{
    if (game->state.mode == MODE_GAME)
    {
        game->state.mode = MODE_INVENTORY;
    }
    else if (game->state.mode == MODE_INVENTORY)
    {
        game->state.mode = MODE_GAME;
    }
}

void Game_Quit(Game_t* game)
{
    World_Save(&game->world);
}


#include "world.h"
#include <stdlib.h>
#include "endian.h"

void Block_Init(Block_t* block)
{
    block->type = BLOCK_AIR;
}

void Chunk_Init(Chunk_t* chunk, short cx, short cy, short cz)
{
    chunk->x = cx;
    chunk->y = cy;
    chunk->z = cz;
    
    chunk->cacheSize = 0;
    
    chunk->saveDirty = 0;
    
    chunk->blockEntityCount = 0;
    chunk->needsToUnload = 0;
    
    int x,y,z;
    for (x = 0; x < CHUNK_SIZE; ++x)
    {
        for (y = 0; y < CHUNK_SIZE; ++y)
        {
            for (z = 0; z < CHUNK_SIZE; ++z)
            {
                Block_t* block = &chunk->blocks[x][y][z];
                Block_Init(block);
                
                if (z == 0)
                {
                    block->type = BLOCK_SOLID;
                }
                else if (z < CHUNK_SIZE / 4)
                {
                    block->type = BLOCK_MUD;
                }
                else if (z < CHUNK_SIZE / 2)
                {
                    block->type = BLOCK_DIRT;
                }
                else if (z < CHUNK_SIZE / 2 + 1)
                {
                    block->type = BLOCK_GRASS;
                }
            }
        }
    }
    
    chunk->worldPosition = Vec3_Create(chunk->x * CHUNK_SIZE, chunk->y * CHUNK_SIZE, chunk->z * CHUNK_SIZE);
    Vec3_t center = Vec3_Add(chunk->worldPosition, Vec3_Create(CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2));
    
    chunk->boundingSphere = Sphere_Create(center, CHUNK_SIZE);
    
    Chunk_Dirty(chunk);
}

void Chunk_Dirty(Chunk_t* chunk)
{
    chunk->dirtyCache = 1;
    chunk->saveDirty = 1;
}

void Entity_Init(Entity_t* entity)
{
    entity->entityID = -1;
    entity->position = Vec3_Zero();
    entity->velocity = Vec3_Zero();
}

void World_Init(World_t* world)
{
    world->chunkCount = 0;
    world->entityCounter = 0;
    
    int i;
    for (i = 0; i < MAX_ENTITIES; i ++)
    {
        Entity_Init(world->entities + i);
    }
    
    world->chunks = NULL;
}

static Chunk_t* _World_AddChunk(World_t* world, int x, int y, int z)
{
    if (!world->chunks)
    {
        world->chunks = malloc(sizeof(Chunk_t));
    }
    else
    {
        world->chunks = realloc(world->chunks, sizeof(Chunk_t) * (world->chunkCount + 1));
    }
    assert(world->chunks);
    
    Chunk_Init(world->chunks + world->chunkCount, x, y, z);
    world->chunkCount++;
    
    return &world->chunks[world->chunkCount - 1];
}

static void _World_GenChunk(World_t* world, Chunk_t* chunk)
{
    Chunk_Init(chunk, chunk->x, chunk->y, chunk->z);
}

Block_t* World_GetBlockAt( World_t* world, int x, int y, int z)
{
    int ix = x / CHUNK_SIZE;
    int iy = y / CHUNK_SIZE;
    int iz = z / CHUNK_SIZE;
    
    int i;
    for (i = 0; i < world->chunkCount; i ++)
    {
        if (world->chunks[i].x == ix &&
            world->chunks[i].y == iy &&
            world->chunks[i].z == iz)
        {
            int bx = x % CHUNK_SIZE;
            int by = y % CHUNK_SIZE;
            int bz = z % CHUNK_SIZE;
            
            return &world->chunks[i].blocks[bx][by][bz];
        }
    }
    
    return NULL;
}

void World_UpdateBlockAt(World_t* world, int x, int y, int z)
{
    int ix = x / CHUNK_SIZE;
    int iy = y / CHUNK_SIZE;
    int iz = z / CHUNK_SIZE;
    
    int i;
    for (i = 0; i < world->chunkCount; i ++)
    {
        if (world->chunks[i].x == ix &&
            world->chunks[i].y == iy &&
            world->chunks[i].z == iz)
        {
            Chunk_Dirty(&world->chunks[i]);
        }
    }
}


void World_PrepareChunk(World_t* world, int ix, int iy, int iz)
{
    
    int i;
    for (i = 0; i < world->chunkCount; i ++)
    {
        if (world->chunks[i].x == ix &&
            world->chunks[i].y == iy &&
            world->chunks[i].z == iz)
        {
            return;
        }
    }
    
    if (!World_LoadChunk(world, ix, iy, iz))
    {
        _World_AddChunk(world, ix, iy, iz);
    }
}

void World_UnloadChunk(World_t* world, int ix, int iy, int iz)
{
    int i;
    for (i = 0; i < world->chunkCount; i ++)
    {
        if (world->chunks[i].x == ix &&
            world->chunks[i].y == iy &&
            world->chunks[i].z == iz)
        {
            //World_SaveChunk(world, world->chunks + i);
            
            /*
            size_t size = (world->chunkCount - (i + 1)) * sizeof(Chunk_t);
            memmove(world->chunks + i, world->chunks + i + 1, size);
            world->chunkCount--;
             */
        }
    }
}


Entity_t* World_SpawnEntity(World_t* world, int type)
{
    Entity_t* entity = world->entities + world->entityCounter;
    entity->type = type;
    entity->entityID = world->entityCounter;
    
    ++world->entityCounter;
    return entity;
}

#define WORLD_STREAM_VERSION 1

void World_Save(World_t* world)
{
    int i;
    for (i = 0; i < world->chunkCount; ++i)
    {
        if (world->chunks[i].saveDirty)
        {
            World_SaveChunk(world, &world->chunks[i]);
        }
    }
}

void World_SaveChunk(World_t* world, Chunk_t* chunk)
{
    assert(chunk);
        
    chunk->saveDirty = 0;

    char filename[1024];
    sprintf(filename, "save/%i_%i_%i.chunk\n", chunk->x, chunk->y, chunk->z);
 
    FILE* file = fopen(filename, "wb");
    
    int32_t version;
    End_I32ToLittle(&version, WORLD_STREAM_VERSION);
    
    fwrite(&version, sizeof(int32_t), 1, file);
    
    int x,y,z;
    for (x = 0; x < CHUNK_SIZE; ++x)
    {
        for (y = 0; y < CHUNK_SIZE; ++y)
        {
            for (z = 0; z < CHUNK_SIZE; ++z)
            {
                Block_t* block = &chunk->blocks[x][y][z];
                fwrite(&block->type, sizeof(char), block->type, file);
            }
        }
    }
    
    fclose(file);
}

int World_LoadChunk(World_t* world, int ix, int iy, int iz)
{
    char filename[1024];
    sprintf(filename, "save/%i_%i_%i.chunk\n", ix, iy, iz);
 
    FILE* file = fopen(filename, "rb");
    
    if (!file)
    {
        return 0;
    }
    
    int32_t version;
    
    fread(&version, sizeof(int32_t), 1, file);
    
    assert(End_I32FromLittle(&version) == WORLD_STREAM_VERSION);
    
    Chunk_t* newChunk = _World_AddChunk(world, ix, iy, iz);
    
    int x,y,z;
    for (x = 0; x < CHUNK_SIZE; ++x)
    {
        for (y = 0; y < CHUNK_SIZE; ++y)
        {
            for (z = 0; z < CHUNK_SIZE; ++z)
            {
                Block_t* block = &newChunk->blocks[x][y][z];
                fread(&block->type, sizeof(char), block->type, file);
            }
        }
    }
    fclose(file);
    
    Chunk_Dirty(newChunk);
    newChunk->saveDirty = 0;
    
    return 1;
}



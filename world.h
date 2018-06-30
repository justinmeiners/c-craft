
#ifndef ccraft_stream_h
#define ccraft_stream_h

#include "vec_math.h"
#include "geo.h"

enum
{
    BLOCK_AIR = 0,
    BLOCK_DIRT,
    BLOCK_GRASS,
    BLOCK_STONE,
    BLOCK_WOOD,
    BLOCK_TABLE,
    BLOCK_BOUNCE_PAD,
    BLOCK_ICE,
    BLOCK_TRACK,
    BLOCK_MUD,
    BLOCK_GIFT,
    BLOCK_SOLID
};

typedef struct
{
    char type;
} Block_t;

extern void Block_Init(Block_t* block);

enum
{
    BLOCK_ENTITY_CHEST = 0,
    BLOCK_ENTITY_TABLE,
};



/* block entities store data assocated with special blocks */
typedef struct
{
    short x;
    short y;
    short z;
    int type;
    void* data;
} BlockEntity_t;

enum
{
    ENTITY_CHEST = 0,
    ENTITY_STONE,
    ENTITY_DIRT,
    ENTITY_TURF,
    ENTITY_GIFT,
};


typedef struct
{
    Vec3_t position;
    Vec3_t velocity;
    float size;
    float height;
    
    int entityID;
    int type;
    int pickupType;
    int qty;
    void* data;
    
} Entity_t;

extern void Entity_Init(Entity_t* entity);


#define CHUNK_SIZE 16


typedef struct
{
    short x;
    short y;
    short z;
    
    float u;
    float v;
} Vert_t;

static inline Vert_t Vert_Create(short x, short y, short z, float u, float v)
{
    Vert_t vert;
    vert.x = x;
    vert.y = y;
    vert.z = z;
    vert.u = u;
    vert.v = v;
    return vert;
}

typedef struct
{
    Vert_t verts[4];
} Face_t;

typedef struct
{
    Block_t blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    BlockEntity_t* blockEntities;
    int blockEntityCount;
    
    int x;
    int y;
    int z;
    
    int dirtyCache;
    int saveDirty;
    int needsToUnload;
    
    Face_t cache[8192];
    int cacheSize;
    
    Vec3_t worldPosition;
    Sphere_t boundingSphere;
        
} Chunk_t;

extern void Chunk_Gen(Chunk_t* chunk);
extern void Chunk_Dirty(Chunk_t* chunk);


#define MAX_ENTITIES 1024

typedef struct
{
    Chunk_t* chunks;
    
    Entity_t entities[MAX_ENTITIES];
    int entityCounter;
    
    int chunkCount;
    int seed;
    
} World_t;

extern void World_Init(World_t* world);
extern Block_t* World_GetBlockAt(World_t* world, int x, int y, int z);
extern void World_UpdateBlockAt(World_t* world, int x, int y, int z);

extern void World_PrepareChunk(World_t* world, int x, int y, int z);
extern void World_UnloadChunk(World_t* world, int x, int y, int z);

extern Entity_t* World_SpawnEntity(World_t* world, int type);

extern void World_Save(World_t* world);

extern void World_SaveChunk(World_t* world, Chunk_t* chunk);
extern int World_LoadChunk(World_t* world, int ix, int iy, int iz);

#endif

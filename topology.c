
#include "topology.h"

#define BLOCK_ATLAS_SIZE 0.0625f
#define BLOCK_ATLAS_ROWS 16

static Vec2_t Atlas_UVForTex(int texID)
{
    Vec2_t uv;
    uv.x = (texID % BLOCK_ATLAS_ROWS) * BLOCK_ATLAS_SIZE;
    uv.y = (texID / BLOCK_ATLAS_ROWS) * BLOCK_ATLAS_SIZE;
    return uv;
}

static int Atlas_TexForBlock(int blockType, int faceID)
{
    switch (blockType)
    {
        case BLOCK_DIRT:
            return 2;
        case BLOCK_GRASS:
        {
            switch (faceID)
            {
                case 0:
                    return 2;
                case 1:
                    return 0;
                default:
                    return 3;
            }
            break;
        }
        case BLOCK_STONE:
            return 16;
        case BLOCK_SOLID:
            return 17;
        case BLOCK_BOUNCE_PAD:
            return 23;
        case BLOCK_ICE:
            return 67;
        case BLOCK_TRACK:
            return 7;
        case BLOCK_MUD:
            return 86;
        case BLOCK_GIFT:
            return 35;
        case BLOCK_TABLE:
        {
            switch (faceID)
            {
                case 0:
                    return 43;
                case 1:
                    return 43;
                case 2:
                    return 60;
                default:
                    return 59;
            }
            break;
        }
    }
    
    return 24;
}


void Topologize_World(Cam_t* cam, World_t* world)
{
    int i;
    for (i = 0; i < world->chunkCount; ++i)
    {
        Chunk_t* chunk = &world->chunks[i];
        
        if (!Cam_SphereVisible(cam, chunk->boundingSphere)) continue;
        
        if (chunk->dirtyCache)
        {
            chunk->cacheSize = 0;
            
            Vec2_t uv;
            int x,y,z;
            
            for (x = 0; x < CHUNK_SIZE; ++x)
            {
                for (y = 0; y < CHUNK_SIZE; ++y)
                {
                    for (z = 0; z < CHUNK_SIZE; ++z)
                    {
                        int type = chunk->blocks[x][y][z].type;
                        if (type == BLOCK_AIR) continue;
                        
                        if (z == 0 || (chunk->blocks[x][y][z - 1].type == BLOCK_AIR))
                        {
                            uv = Atlas_UVForTex(Atlas_TexForBlock(type, 0));
                            
                            chunk->cache[chunk->cacheSize].verts[3] = Vert_Create(x, y, z, uv.x, uv.y);
                            chunk->cache[chunk->cacheSize].verts[2] = Vert_Create(x + 1, y, z, uv.x + BLOCK_ATLAS_SIZE, uv.y);
                            chunk->cache[chunk->cacheSize].verts[1] = Vert_Create(x + 1, y + 1, z, uv.x + BLOCK_ATLAS_SIZE, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[0] = Vert_Create(x, y + 1, z, uv.x, uv.y + BLOCK_ATLAS_SIZE);
                            ++chunk->cacheSize;
                        }
                        
                        if (z == CHUNK_SIZE - 1 || (chunk->blocks[x][y][z + 1].type == BLOCK_AIR))
                        {
                            uv = Atlas_UVForTex(Atlas_TexForBlock(type, 1));
                            
                            chunk->cache[chunk->cacheSize].verts[0] = Vert_Create(x, y, z + 1, uv.x, uv.y);
                            chunk->cache[chunk->cacheSize].verts[1] = Vert_Create(x + 1, y, z + 1, uv.x + BLOCK_ATLAS_SIZE, uv.y);
                            chunk->cache[chunk->cacheSize].verts[2] = Vert_Create(x + 1, y + 1, z + 1, uv.x + BLOCK_ATLAS_SIZE, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[3] = Vert_Create(x, y + 1, z + 1, uv.x, uv.y + BLOCK_ATLAS_SIZE);
                            ++chunk->cacheSize;
                        }
                        
                        
                        if (y == 0 || (chunk->blocks[x][y - 1][z].type == BLOCK_AIR))
                        {
                            uv = Atlas_UVForTex(Atlas_TexForBlock(type, 4));
                            
                            chunk->cache[chunk->cacheSize].verts[0] = Vert_Create(x, y, z, uv.x, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[1] = Vert_Create(x + 1, y, z, uv.x + BLOCK_ATLAS_SIZE, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[2] = Vert_Create(x + 1, y, z + 1, uv.x + BLOCK_ATLAS_SIZE, uv.y);
                            chunk->cache[chunk->cacheSize].verts[3] = Vert_Create(x, y, z + 1, uv.x, uv.y);
                            ++chunk->cacheSize;
                        }
                        
                        if (y == CHUNK_SIZE - 1 || (chunk->blocks[x][y + 1][z].type == BLOCK_AIR))
                        {
                            uv = Atlas_UVForTex(Atlas_TexForBlock(type, 5));
                            
                            chunk->cache[chunk->cacheSize].verts[3] = Vert_Create(x, y + 1, z, uv.x, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[2] = Vert_Create(x + 1, y + 1, z, uv.x + BLOCK_ATLAS_SIZE, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[1] = Vert_Create(x + 1, y + 1, z + 1, uv.x + BLOCK_ATLAS_SIZE, uv.y);
                            chunk->cache[chunk->cacheSize].verts[0] = Vert_Create(x, y + 1, z + 1, uv.x, uv.y);
                            ++chunk->cacheSize;
                        }
                        
                        if (x == 0 || (chunk->blocks[x - 1][y][z].type == BLOCK_AIR))
                        {
                            uv = Atlas_UVForTex(Atlas_TexForBlock(type, 2));
                            
                            chunk->cache[chunk->cacheSize].verts[3] = Vert_Create(x, y, z, uv.x, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[2] = Vert_Create(x, y + 1, z, uv.x + BLOCK_ATLAS_SIZE, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[1] = Vert_Create(x, y + 1, z + 1, uv.x + BLOCK_ATLAS_SIZE, uv.y);
                            chunk->cache[chunk->cacheSize].verts[0] = Vert_Create(x, y, z + 1, uv.x, uv.y);
                            ++chunk->cacheSize;
                        }
                        
                        if (x == CHUNK_SIZE - 1 || (chunk->blocks[x + 1][y][z].type == BLOCK_AIR))
                        {
                            uv = Atlas_UVForTex(Atlas_TexForBlock(type, 3));
                            
                            chunk->cache[chunk->cacheSize].verts[0] = Vert_Create(x + 1, y, z, uv.x, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[1] = Vert_Create(x + 1, y + 1, z, uv.x + BLOCK_ATLAS_SIZE, uv.y + BLOCK_ATLAS_SIZE);
                            chunk->cache[chunk->cacheSize].verts[2] = Vert_Create(x + 1, y + 1, z + 1, uv.x + BLOCK_ATLAS_SIZE, uv.y);
                            chunk->cache[chunk->cacheSize].verts[3] = Vert_Create(x + 1, y, z + 1, uv.x, uv.y);
                            ++chunk->cacheSize;
                        }
                    }
                }
            }
            chunk->dirtyCache = 0;
        }
    }
}
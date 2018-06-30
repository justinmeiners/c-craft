
#include "endian.h"

int End_IsLittle()
{
    union
    {
        uint8_t bytes[4];
        uint32_t i;
    } u;
    
    u.i = 1;
    
    if (u.bytes[0] == 1)
    {
        return 1;
    }
    
    return 0;
}

void End_U16ToLittle(void* dst, uint16_t val)
{
    uint8_t* p = (uint8_t*)dst;
    
    p[0] = val & 0xFF;
    p[1] = (val & 0xFF00) >> 8;
}

void End_I16ToLittle(void* dst, int16_t val)
{
    End_U16ToLittle(dst, (uint16_t)val);
}

void End_U32ToLittle(void* dst, uint32_t val)
{
    
    uint8_t* p = (uint8_t*)dst;
    
    p[0] = (val & 0xFF);
    p[1] = (val & 0xFF00) >> 8;
    p[2] = (val & 0xFF0000) >> 16;
    p[3] = (val & 0xFF000000) >> 24;
}

void End_I32ToLittle(void* dst, int32_t val)
{
    End_U32ToLittle(dst, (uint32_t)val);
}

void End_U64ToLittle(void* dst, uint64_t val)
{
    uint8_t* p  = (uint8_t*)dst;
    
    for (int i = 0; i < 8; i++, val >>= 8 )
    {
        p[i] = (uint8_t)(val & 0xFF);
    }
}

void End_I64ToLittle(void* dst, int64_t val)
{
    End_U64ToLittle(dst, (uint32_t)val);
}

uint16_t End_U16FromLittle(const void* src)
{
    uint16_t v = 0;
    uint8_t* p = (uint8_t*)src;
    
    v |= p[0];
    v |= ((uint16_t)p[1]) << 8;
    
    return v;
}

int16_t End_I16FromLittle(const void* src)
{
    return (int16_t)End_U16FromLittle(src);
}

uint32_t End_U32FromLittle(const void* src)
{
    uint32_t v = 0;
    uint8_t* p = (uint8_t*)src;
    
    v |= p[0];
    v |= ((uint32_t)p[1]) << 8;
    v |= ((uint32_t)p[2]) << 16;
    v |= ((uint32_t)p[3]) << 24;
    
    return v;
}

int32_t End_I32FromLittle(const void* src)
{
    return (int32_t)End_U32FromLittle(src);
}

uint64_t End_U64FromLittle(const void* src)
{
    uint64_t v = 0;
    uint8_t* p = (uint8_t*)src;
    
    for (int i = 0; i < 8; i++ )
    {
        v |= ((uint64_t)p[i]) << (i * 8);
    }
    
    return v;
}

int64_t End_I64FromLittle(const void* src)
{
    return (int64_t)End_U64FromLittle(src);
}

void End_F32ToLittle(void* dst, float val)
{
    union
    {
        float f32;
        uint32_t u32;
    } u;
    
    u.f32 = val;
    
    End_U32ToLittle(dst, u.u32);
}

float End_F32FromLittle(const void* src)
{
    union
    {
        float f32;
        uint32_t u32;
    } u;
    
    u.u32 = End_U32FromLittle(src);
    return u.f32;
}


void End_U16ToBig(void* dst, uint16_t val)
{
    uint8_t* p  = (uint8_t*)dst;
    
    p[1] = (val & 0xFF);
    p[0] = (val & 0xFF00) >> 8;
}

void End_I16ToBig(void* dst, int16_t val)
{
    End_U16ToBig(dst, (uint16_t)val);
}

void End_U32ToBig(void* dst, uint32_t val)
{
    uint8_t* p  = (uint8_t*)dst;
    
    p[3] = (val & 0xFF);
    p[2] = (val & 0xFF00) >> 8;
    p[1] = (val & 0xFF0000) >> 16;
    p[0] = (val & 0xFF000000) >> 24;
}

void End_I32ToBig(void* dst, int32_t val)
{
    End_U32ToBig(dst, (uint32_t)val);
}

void End_U64ToBig(void* dst, uint64_t val)
{
    uint8_t* p  = (uint8_t*)dst;
    
    for (int i = 0; i < 8; i++, val >>= 8 )
    {
        p[7 - i] = (uint8_t)(val & 0xFF);
    }
}

void End_I64ToBig(void* dst, int64_t val)
{
    End_U64ToBig(dst, (uint64_t)val);
}

uint16_t End_U16FromBig(const void* src)
{
    uint16_t v = 0;
    uint8_t* p = (uint8_t*)src;
    
    v |= p[1];
    v |= ((uint16_t)p[0]) << 8;
    
    return v;
}

int16_t End_I16FromBig(const void* src)
{
    return (uint16_t)End_U16FromBig(src);
}

uint32_t End_U32FromBig(const void* src)
{
    uint32_t v = 0;
    uint8_t* p = (uint8_t*)src;
    
    v |= p[3];
    v |= ((uint32_t)p[2]) << 8;
    v |= ((uint32_t)p[1]) << 16;
    v |= ((uint32_t)p[0]) << 24;
    
    return v;
}

int32_t End_I32FromBig(const void* src)
{
    return (int32_t)End_U32FromBig(src);
}

uint64_t End_U64FromBig(const void* src)
{
    uint64_t v = 0;
    uint8_t* p = (uint8_t*)src;
    
    for (int i = 0; i < 8; i++ )
    {
        v |= ((uint64_t)p[7 - i]) << (i * 8);
    }
    
    return v;
}

int64_t End_I64FromBig(const void* src)
{
    return (int64_t)End_U64FromBig(src);
}

void End_F32ToBig(void* dst, float val)
{
    union
    {
        float f32;
        uint32_t u32;
    } u;
    
    u.f32 = val;
    
    End_U32ToBig(dst, u.u32);
}

float End_F32FromBig(const void* src)
{
    union
    {
        float f32;
        uint32_t u32;
    } u;
    
    u.u32 = End_U32FromBig(src);
    return u.f32;
}


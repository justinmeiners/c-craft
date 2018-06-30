
#ifndef GEO_H
#define GEO_H

#include "vec_math.h"
#include <assert.h>

#define TRUE 1
#define FALSE 0

typedef struct Ray2
{
    Vec2_t origin;
    Vec2_t direction;
} Ray2_t;

static inline Ray2_t Ray2_Create(Vec2_t origin, Vec2_t direction)
{
    Ray2_t ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
}

typedef struct Ray3
{
    Vec3_t origin;
    Vec3_t direction;
} Ray3_t;

static inline Ray3_t Ray3_Create(Vec3_t origin, Vec3_t dir)
{
    Ray3_t ray;
    ray.origin = origin;
    ray.direction = dir;
    return ray;
}

typedef struct Plane
{
    Vec3_t position;
    Vec3_t normal;
} Plane_t;

static inline Plane_t Plane_Create(Vec3_t pos, Vec3_t norm)
{
    Plane_t p;
    p.position = pos;
    p.normal = norm;
    return p;
}

static inline float Plane_SignedDist(Plane_t p, Vec3_t a)
{
    return Vec3_Dot(p.normal, Vec3_Sub(a, p.position));
}

static inline int Plane_IntersectRay(Plane_t plane, Ray3_t ray, float* t)
{
    float d = Vec3_Dot(plane.position, plane.normal);
    
    float num = Vec3_Dot(plane.normal, ray.origin) + d;
    float denom = Vec3_Dot(plane.normal, ray.direction);
    
    if (fabsf(denom) < 0.00001f)
    {
        return 0;
    }
    
    *t = -(num / denom);
    
    return 1;
}

static inline float Plane_SignedDistToPoint(Plane_t plane, Vec3_t point)
{
    return Vec3_Dot(plane.normal, Vec3_Sub(point, plane.position));
}

typedef struct AABB
{
    Vec3_t min;
    Vec3_t max;
} AABB_t;

static inline AABB_t AABB_Zero()
{
    AABB_t a;
    a.min = Vec3_Zero();
    a.max = Vec3_Zero();
    return a;
}

static inline AABB_t AABB_Create(Vec3_t min, Vec3_t max)
{
    AABB_t a;
    a.min = min;
    a.max = max;
    return a;
}

static inline int AABB_IntersectsAABB(AABB_t a, AABB_t b)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        if (a.max.data[i] < b.min.data[i])
        {
            return FALSE;
        }
        
        if (a.min.data[i] > b.max.data[i])
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

static inline int AABB_IntersectsPoint(AABB_t a, Vec3_t p)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        if (p.data[i] < a.min.data[i])
        {
            return FALSE;
        }
        
        if (p.data[i] > a.max.data[i])
        {
            return FALSE;
        }
        
    }
    return TRUE;
}

static inline Vec3_t AABB_Center(AABB_t a)
{
    Vec3_t center;
    int i;
    for (i = 0; i < 3; ++i)
    {
        center.data[i] = (a.min.data[i] + a.max.data[i]) * 0.5f;
    }
    return center;
}

static inline int AABB_IntersectsPlane(AABB_t aabb, Plane_t plane)
{
    Vec3_t p = aabb.min;
    
    if (plane.normal.x >= 0)
    {
        p.x = aabb.max.x;
    }
    if (plane.normal.y >= 0)
    {
        p.y = aabb.max.y;
    }
    if (plane.normal.z >= 0)
    {
        p.z = aabb.max.z;
    }
    
    if (Vec3_Dot(plane.normal, Vec3_Sub(p, plane.position)) < 0.0f)
    {
        return FALSE;
    }
    return TRUE;
    
}

typedef struct Sphere
{
    Vec3_t position;
    float radius;
} Sphere_t;

static inline Sphere_t Sphere_Create(Vec3_t position, float radius)
{
    Sphere_t s;
    s.position = position;
    s.radius = radius;
    return s;
}

static inline int Sphere_IntersectsPoint(Sphere_t a, Vec3_t point)
{
    float r = a.radius;
    
    Vec3_t diff = Vec3_Sub(a.position, point);
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    
    if (distSq < r)
    {
        return 1;
    }
    
    return 0;
}

static inline int Sphere_IntersectsAABB(Sphere_t a, AABB_t b)
{
    Vec3_t min = Vec3_Sub(b.min, a.position);
    Vec3_t max = Vec3_Sub(b.max, a.position);
    
    int i;
    for (i = 0; i < 3; ++i)
    {
        if (max.data[i] < -a.radius)
        {
            return FALSE;
        }
        if (min.data[i] > a.radius)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

static inline int Sphere_IntersectsSphere(Sphere_t a, Sphere_t b)
{
    float r = a.radius + b.radius;
    
    Vec3_t diff = Vec3_Sub(a.position, b.position);
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    
    if (distSq < r)
    {
        return 1;
    }
    
    return 0;
}

static inline int Sphere_IntersectsPlane(Sphere_t a, Plane_t p)
{
    if (Plane_SignedDist(p, a.position) < a.radius)
    {
        return TRUE;
    }
    
    return FALSE;
}

extern int Sphere_IntersectsRay(Sphere_t a, Ray3_t ray, float* t);

extern int Geo_PointInPoly(int nvert, Vec2_t* verts, Vec2_t test);

extern Vec3_t Geo_BlendBarcentric(Vec3_t p1,
                                  Vec3_t p2,
                                  Vec3_t p3,
                                  Vec2_t i);

#endif

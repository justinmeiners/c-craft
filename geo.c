
#include "geo.h"

int Sphere_IntersectsRay(Sphere_t a, Ray3_t ray, float* t)
{
    float det;
    Vec3_t p = Vec3_Sub(ray.origin, a.position);
    float b = - Vec3_Dot(p, ray.direction);
    det = b * b - Vec3_Dot(p, p) + a.radius * a.radius;
    
    if (det < 0)
    {
        return 0;
    }
    
    det = sqrtf(det);
    float i1 = b - det;
    float i2 = b + det;
    
    if (i2 < 0)
    {
        return 0;
    }
    if (i1 < 0)
    {
        i1 = 0;
    }
    
    *t = i1;
    
    return 1;
}

/*
 magic PNPOLY - Point Inclusion in Polygon Test
 W. Randolph Franklin (WRF) - http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
 */

int Geo_PointInPoly(int nvert, Vec2_t* verts, Vec2_t test)
{
    int i, j, c = 0;
    for (i = 0, j = nvert - 1; i < nvert; j = i++)
    {
        if ( ((verts[i].y > test.y) != (verts[j].y > test.y)) &&
            (test.x < (verts[j].x - verts[i].x) * (test.y - verts[i].y) / (verts[j].y - verts[i].y) + verts[i].x) )
        {
            c = !c;
        }
    }
    return c;
}

Vec3_t Geo_BlendBarcentric(Vec3_t p1,
                           Vec3_t p2,
                           Vec3_t p3,
                           Vec2_t i)
{
    Vec3_t ret;
    float b0 = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) - (p2.y - p1.y);
    
    ret.x = ((p2.x - i.x) * (p3.y - i.y) * (p3.x - i.x) * (p2.y - i.y)) / b0;
    ret.y = ((p3.x - i.x) * (p1.y - i.y) * (p1.x - i.x) * (p3.y - i.y)) / b0;
    ret.z = ((p1.x - i.x) * (p2.y - i.y) * (p2.x - i.x) * (p1.y - i.y)) / b0;
    
    return ret;
}

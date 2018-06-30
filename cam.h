
#ifndef CAM_H
#define CAM_H

#include "vec_math.h"
#include "geo.h"

/* plane indices */
enum
{
    CAM_TOP_PLANE = 0,
    CAM_BOTTOM_PLANE = 1,
    CAM_LEFT_PLANE = 2,
    CAM_RIGHT_PLANE = 3,
    CAM_NEAR_PLANE = 4,
    CAM_FAR_PLANE = 5,
};

/* basic 3D perspective camera with frustum culling */

typedef struct Cam
{
    Vec3_t position;
    Vec3_t target;
    Vec3_t orientation;
    
    Vec3_t right;
    Vec3_t up;
    Vec3_t view;
    
    float fov;
    float near;
    float far;
    
    Mat4_t projectionMat;
    Mat4_t viewMat;
    Plane_t planes[6];
    
} Cam_t;

extern void Cam_Init(Cam_t* cam);
extern void Cam_Shutdown(Cam_t* cam);

/* rebuild frustum and matrices - viewWidth and viewHeight are simply for calculting aspect ratio */
extern void Cam_UpdateTransform(Cam_t* camera, short viewWidth, short viewHeight);

/* precalculated matricies */
extern const Mat4_t* Cam_ViewMat(const Cam_t* camera);
extern const Mat4_t* Cam_ProjectionMat(const Cam_t* camera);

/* visiblity testing */
extern int Cam_SphereVisible(const Cam_t* camera, Sphere_t sphere);
extern int Cam_PointVisible(const Cam_t* camera, Vec3_t point);

#endif

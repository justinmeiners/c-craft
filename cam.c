
#include "cam.h"

void Cam_Init(Cam_t* cam)
{
    cam->fov = 45.0f;
    cam->near = 1.0f;
    cam->far = 100.0f;
    cam->position = Vec3_Zero();
    cam->target = Vec3_Zero();
    cam->orientation = Vec3_Create(0.0f, 0.0f, 1.0f);
    Mat4_Identity(&cam->projectionMat);
    Mat4_Identity(&cam->viewMat);
}

void Cam_Shutdown(Cam_t* cam)
{
}

void Cam_UpdateTransform(Cam_t* camera, short viewWidth, short viewHeight)
{
    assert(camera);
    
    const float aspectRatio = (float)viewWidth / (float)viewHeight;
    const float tang = tanf((camera->fov * M_PI / 180.0f) * 0.5f);
    
    const float nearHeight = camera->near * tang;
    const float nearWidth = nearHeight * aspectRatio;
    
    /* compute matrices */
    camera->viewMat = Mat4_CreateLook(camera->position, camera->target, camera->orientation);
    camera->projectionMat = Mat4_CreateFrustum(-nearWidth, nearWidth, -nearHeight, nearHeight, camera->near, camera->far);

    /* determine orientation vectors */
    camera->view = Vec3_Normalize(Vec3_Sub(camera->position, camera->target));
	camera->right = Vec3_Normalize(Vec3_Cross(camera->orientation, camera->view));
	
	camera->up = Vec3_Cross(camera->view, camera->right);
    
    Vec3_t fc = Vec3_Sub(camera->position, Vec3_Scale(camera->view, camera->far));
    Vec3_t nc = Vec3_Sub(camera->position, Vec3_Scale(camera->view, camera->near));
    
    /* complicated code builds planes for frustum */
    Plane_t* planes = camera->planes;
    
    planes[CAM_NEAR_PLANE].normal = Vec3_Negate(camera->view);
    planes[CAM_NEAR_PLANE].position = nc;
    
    planes[CAM_FAR_PLANE].normal = camera->view;
    planes[CAM_FAR_PLANE].position = fc;
    
    Vec3_t aux, aux2, normal;
    
    aux2 = Vec3_Add(nc, Vec3_Scale(camera->up, nearHeight));
    aux = Vec3_Normalize(Vec3_Sub(aux2, camera->position));
    normal = Vec3_Cross(aux, camera->right);
    planes[CAM_TOP_PLANE].normal = normal;
    planes[CAM_TOP_PLANE].position = aux2;
    
    aux2 = Vec3_Sub(nc, Vec3_Scale(camera->up, nearHeight));
    aux = Vec3_Normalize(Vec3_Sub(aux2, camera->position));
    aux = Vec3_Normalize(aux);
    normal = Vec3_Cross(camera->right, aux);
    planes[CAM_BOTTOM_PLANE].normal = normal;
    planes[CAM_BOTTOM_PLANE].position = aux2;
    
    aux2 = Vec3_Sub(nc, Vec3_Scale(camera->right, nearWidth));
    aux = Vec3_Normalize(Vec3_Sub(aux2, camera->position));
    normal = Vec3_Cross(aux, camera->up);
    planes[CAM_LEFT_PLANE].normal = normal;
    planes[CAM_LEFT_PLANE].position = aux2;
    
    aux2 = Vec3_Add(nc, Vec3_Scale(camera->right, nearWidth));
    aux = Vec3_Normalize(Vec3_Sub(aux2, camera->position));
    normal = Vec3_Cross(camera->up, aux);
    planes[CAM_RIGHT_PLANE].normal = normal;
    planes[CAM_RIGHT_PLANE].position = aux2;
}

const Mat4_t* Cam_ViewMat(const Cam_t* camera)
{
    assert(camera);
    return &camera->viewMat;
}

const Mat4_t* Cam_ProjectionMat(const Cam_t* camera)
{
    assert(camera);
    return &camera->projectionMat;
}

int Cam_SphereVisible(const Cam_t* camera, Sphere_t sphere)
{
    assert(camera);
    int i;
    for (i = 0; i < 6; ++i)
    {
		if (Vec3_Dot(camera->planes[i].normal, Vec3_Sub(sphere.position, camera->planes[i].position)) < -sphere.radius)
        {
            return 0;
        }
	}
	return 1;
}

int Cam_PointVisible(const Cam_t* camera, Vec3_t point)
{
    assert(camera);
    int i;
    for (i = 0; i < 6; i++)
    {
		if (Vec3_Dot(camera->planes[i].normal, Vec3_Sub(point, camera->planes[i].position)) < 0)
        {
            return 0;
        }
	}
	return 1;
}


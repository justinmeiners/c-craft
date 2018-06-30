
#ifndef VEC_MATH_H
#define VEC_MATH_H

#include <math.h>
#include <string.h>
#include <stdio.h>

/* Common 3D Math */

#define DEG_TO_RAD(X) ((X * M_PI) / 180.0f)
#define RAD_TO_DEG(X) ((X * 180.0f) / M_PI)

#define CLAMP(VAL, MIN, MAX) (VAL < MIN ? MIN: (VAL > MAX ? MAX : VAL))

static inline float Lerp (float t, float min, float max)
{
    return t * max + (1.0f - t) * min;
}

static inline float InvLerp (float val, float min, float max)
{
    return (val - min) / max;
}

/* Vectors make use of a union to allow elements to be accessed by name or index */
typedef struct Vec2
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        
        float data[2];
    };
} Vec2_t;


static inline Vec2_t Vec2_Zero()
{
    Vec2_t vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    return vec;
}

static inline Vec2_t Vec2_Create(float x, float y)
{
    Vec2_t vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

static inline Vec2_t Vec2_Clear(float val)
{
    Vec2_t vec;
    vec.x = val;
    vec.y = val;
    return vec;
}

static inline Vec2_t Vec2_Add(Vec2_t a, Vec2_t b)
{
    Vec2_t vec;
    vec.x = a.x + b.x;
    vec.y = a.y + b.y;
    return vec;
}

static inline Vec2_t Vec2_Sub(Vec2_t a, Vec2_t b)
{
    Vec2_t vec;
    vec.x = a.x - b.x;
    vec.y = a.y - b.y;
    return vec;
}

static inline Vec2_t Vec2_Scale(Vec2_t a, float scale)
{
    Vec2_t vec;
    vec.x = a.x * scale;
    vec.y = a.y * scale;
    return vec;
}

static inline Vec2_t Vec2_Negate(Vec2_t a)
{
    Vec2_t vec;
    vec.x = -a.x;
    vec.y = -a.y;
    return vec;
}

static inline float Vec2_ToAngle(Vec2_t a)
{
    return atan2f(a.y, a.x);
}

static inline float Vec2_Length(Vec2_t vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

static inline float Vec2_LengthSq(Vec2_t vec)
{
    return (vec.x * vec.x + vec.y * vec.y);
}

static inline float Vec2_Dist(Vec2_t a, Vec2_t b)
{
    return Vec2_Length(Vec2_Sub(a, b));
}

static inline float Vec2_ManhatDist(Vec2_t a, Vec2_t b)
{
    return fabsf(a.x - b.x) + fabsf(a.y - b.y);
}

static inline Vec2_t Vec2_Normalize(Vec2_t a)
{
    float s = 1.0f / Vec2_Length(a);
    
    Vec2_t n;
    n.x = a.x * s;
    n.y = a.y * s;
    return n;
}

static inline float Vec2_Dot(Vec2_t a, Vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}

static inline Vec2_t Vec2_Cross(Vec2_t a)
{
    return Vec2_Create(a.y, -a.x);
}

static inline Vec2_t Vec2_Lerp(Vec2_t a, Vec2_t b, float t)
{
    Vec2_t v;
    v.x = Lerp(t, a.x, b.x);
    v.y = Lerp(t, a.y, b.y);
    return v;
}

typedef struct Vec3
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        struct
        {
            float r;
            float g;
            float b;
        };
        
        float data[3];
    };
} Vec3_t;

static inline void Vec3_Print(Vec3_t v)
{
    printf("%f, %f, %f\n", v.x, v.y, v.z);
}

static inline Vec3_t Vec3_Zero()
{
    Vec3_t vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;
    return vec;
}

static inline Vec3_t Vec3_Create(float x, float y, float z)
{
    Vec3_t vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}

static inline Vec3_t Vec3_FromVec2(Vec2_t vec)
{
    return Vec3_Create(vec.x, vec.y, 0.0f);
}

static inline Vec3_t Vec3_Clear(float val)
{
    Vec3_t vec;
    vec.x = val;
    vec.y = val;
    vec.z = val;
    return vec;
}

static inline Vec3_t Vec3_Add(Vec3_t a, Vec3_t b)
{
    Vec3_t vec;
    vec.x = a.x + b.x;
    vec.y = a.y + b.y;
    vec.z = a.z + b.z;
    return vec;
}

static inline Vec3_t Vec3_Sub(Vec3_t a, Vec3_t b)
{
    Vec3_t vec;
    vec.x = a.x - b.x;
    vec.y = a.y - b.y;
    vec.z = a.z - b.z;
    return vec;
}

static inline Vec3_t Vec3_Mult(Vec3_t a, Vec3_t b)
{
    Vec3_t vec;
    vec.x = a.x * b.x;
    vec.y = a.y * b.y;
    vec.z = a.z * b.z;
    return vec;
}

static inline Vec3_t Vec3_Scale(Vec3_t a, float scale)
{
    Vec3_t vec;
    vec.x = a.x * scale;
    vec.y = a.y * scale;
    vec.z = a.z * scale;
    return vec;
}

static inline Vec3_t Vec3_Mid(Vec3_t a, Vec3_t b)
{
    Vec3_t vec;
    vec.x = (a.x + b.x) * 0.5f;
    vec.y = (a.y + b.y) * 0.5f;
    vec.z = (a.z + b.z) * 0.5f;
    return vec;
}

static inline float Vec3_Length(Vec3_t a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline float Vec3_LengthSq(Vec3_t a)
{
    return (a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline float Vec3_Dist(Vec3_t a, Vec3_t b)
{
    return Vec3_Length(Vec3_Sub(a, b));
}

static inline float Vec3_DistSq(Vec3_t a, Vec3_t b)
{
    return Vec3_LengthSq(Vec3_Sub(a, b));
}

static inline float Vec3_ManhatLength(Vec3_t a)
{
    return fabsf(a.x) + fabsf(a.y) + fabsf(a.z);
}

static inline float Vec3_ManhatDist(Vec3_t a, Vec3_t b)
{
    return fabsf(a.x - b.x) + fabsf(a.y - b.y) + fabsf(a.z - b.z);
}

static inline Vec3_t Vec3_Normalize(Vec3_t a)
{
    float s = 1.0f / Vec3_Length(a);

    Vec3_t n;
    n.x = a.x * s;
    n.y = a.y * s;
    n.z = a.z * s;
    return n;
}

static inline Vec3_t Vec3_Negate(Vec3_t a)
{
    Vec3_t vec;
    vec.x = -a.x;
    vec.y = -a.y;
    vec.z = -a.z;
    return vec;
}

static inline float Vec3_Dot(Vec3_t a, Vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline Vec3_t Vec3_Cross(Vec3_t a, Vec3_t b)
{
    Vec3_t vec;
	vec.x = a.y * b.z - a.z * b.y;
	vec.y = a.z * b.x - a.x * b.z;
	vec.z = a.x * b.y - a.y * b.x;
	return vec;
}

static inline Vec3_t Vec3_Lerp(Vec3_t a, Vec3_t b, float t)
{
    Vec3_t v;
    v.x = Lerp(t, a.x, b.x);
    v.y = Lerp(t, a.y, b.y);
    v.z = Lerp(t, a.z, b.z);
    return v;
}

static inline Vec2_t Vec3_ToVec2(Vec3_t a)
{
    return Vec2_Create(a.x, a.y);
}

typedef struct Vec4
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
        
        float data[4];
    };
} Vec4_t;

static inline Vec4_t Vec4_Zero()
{
    Vec4_t vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;
    vec.w = 0.0f;
    return vec;
}

static inline Vec4_t Vec4_Create(float x, float y, float z, float w)
{
    Vec4_t vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = w;
    return vec;
}

static inline Vec4_t Vec4_Add(Vec4_t a, Vec4_t b)
{
    Vec4_t vec;
    vec.x = a.x + b.x;
    vec.y = a.y + b.y;
    vec.z = a.z + b.z;
    vec.w = a.w + b.w;
    return vec;
}

static inline Vec4_t Vec4_Sub(Vec4_t a, Vec4_t b)
{
    Vec4_t vec;
    vec.x = a.x - b.x;
    vec.y = a.y - b.y;
    vec.z = a.z - b.z;
    vec.w = a.w - b.w;
    return vec;
}

typedef struct
{
    float m[16];
} Mat4_t;

extern void Mat4_Mult(const Mat4_t* a, const Mat4_t* b, Mat4_t* dest);
extern void Mat4_Transpose(Mat4_t* a);
extern float Mat4_Det(const Mat4_t* matrix);
extern void Mat4_Inverse(const Mat4_t* matrix, Mat4_t* ret);

extern Mat4_t Mat4_CreateLook(Vec3_t eye,
                              Vec3_t target,
                              Vec3_t up);

extern Mat4_t Mat4_CreateFrustum(float left,
                                 float right,
                                 float bottom,
                                 float top,
                                 float near,
                                 float far);

static inline float Mat4_Get(const Mat4_t* mat, int x, int y)
{
    return mat->m[x * 4 + y];
}

static inline void Mat4_Set(Mat4_t* mat, int x, int y, float val)
{
    mat->m[x * 4 + y] = val;
}

static inline void Mat4_Copy(Mat4_t* dest, const Mat4_t* source)
{
    memcpy(dest, source, sizeof(Mat4_t));
}

static inline void Mat4_Identity(Mat4_t* mat)
{
    int i;
    for (i = 0; i < 16; ++i)
    {
        mat->m[i] = ((i % 5) ? 0 : 1);
    }
}

static inline Vec3_t Mat4_MultVec3(const Mat4_t* mat, Vec3_t a)
{
    Vec3_t vec;
    vec.x = (a.x * mat->m[0]) + (a.y * mat->m[4]) + (a.z * mat->m[8]) + mat->m[12];
    vec.y = (a.x * mat->m[1]) + (a.y * mat->m[5]) + (a.z * mat->m[9]) + mat->m[13];
    vec.z = (a.x * mat->m[2]) + (a.y * mat->m[6]) + (a.z * mat->m[10]) + mat->m[14];
    return vec;
}

static inline Vec4_t Mat4_MultVec4(const Mat4_t* mat, Vec4_t a)
{
    Vec4_t vec;
    vec.x = (a.x * mat->m[0]) + (a.y * mat->m[4]) + (a.z * mat->m[8]) + (a.w * mat->m[12]);
    vec.y = (a.x * mat->m[1]) + (a.y * mat->m[5]) + (a.z * mat->m[9]) + (a.w * mat->m[13]);
    vec.z = (a.x * mat->m[2]) + (a.y * mat->m[6]) + (a.z * mat->m[10]) + (a.w * mat->m[14]);
    vec.w = (a.x * mat->m[3]) + (a.y * mat->m[7]) + (a.z * mat->m[11]) + (a.w * mat->m[15]);
    return vec;
}

static inline void Mat4_Translate(Mat4_t* m, Vec3_t trans)
{
    Mat4_Set(m, 3, 0, Mat4_Get(m, 0, 0) * trans.x + Mat4_Get(m, 1, 0) * trans.y + Mat4_Get(m, 2, 0) * trans.z + Mat4_Get(m, 3, 0));
    Mat4_Set(m, 3, 1, Mat4_Get(m, 0, 1) * trans.x + Mat4_Get(m, 1, 1) * trans.y + Mat4_Get(m, 2, 1) * trans.z + Mat4_Get(m, 3, 1));
    Mat4_Set(m, 3, 2, Mat4_Get(m, 0, 2) * trans.x + Mat4_Get(m, 1, 2) * trans.y + Mat4_Get(m, 2, 2) * trans.z + Mat4_Get(m, 3, 2));
    Mat4_Set(m, 3, 3, Mat4_Get(m, 0, 3) * trans.x + Mat4_Get(m, 1, 3) * trans.y + Mat4_Get(m, 2, 3) * trans.z + Mat4_Get(m, 3, 3));
}

static inline void Mat4_Scale(Mat4_t* mat, Vec3_t scale)
{
    mat->m[0] *= scale.x;
	mat->m[4] *= scale.x;
	mat->m[8] *= scale.x;
	mat->m[12] *= scale.x;
	
	mat->m[1] *= scale.y;
	mat->m[5] *= scale.y;
	mat->m[9] *= scale.y;
	mat->m[13] *= scale.y;
	
	mat->m[2] *= scale.z;
	mat->m[6] *= scale.z;
	mat->m[10] *= scale.z;
	mat->m[14] *= scale.z;
}

static inline Mat4_t Mat4_CreateTranslate(Vec3_t translate)
{
    Mat4_t m;
    Mat4_Identity(&m);
    
    Mat4_Set(&m, 3, 0, translate.x);
    Mat4_Set(&m, 3, 1, translate.y);
    Mat4_Set(&m, 3, 2, translate.z);
    return m;
}

static inline Vec3_t Mat4_Unproject(const Mat4_t* inverseMvp, Vec2_t normalized, float depth)
{
    Vec4_t pointClip = Mat4_MultVec4(inverseMvp, Vec4_Create(normalized.x, normalized.y, depth, 1.0f));
    return Vec3_Scale(Vec3_Create(pointClip.x, pointClip.y, pointClip.z), 1.0f / pointClip.w);
}

static inline Vec3_t Mat4_Project(const Mat4_t* mvp, Vec3_t point)
{
    Vec4_t point3D = Mat4_MultVec4(mvp, Vec4_Create(point.x, point.y, point.z, 1.0f));
    return Vec3_Scale(Vec3_Create(point3D.x, point3D.y, point3D.z), 1.0f / point3D.w);
}

typedef struct Quat
{
    float x;
    float y;
    float z;
    float w;
} Quat_t;


extern Quat_t Quat_FromEuler(float pitch, float yaw, float roll);
extern Quat_t Quat_FromAngle(float angle, float x, float y, float z);

extern void Quat_ToMatrix(Quat_t a, Mat4_t* dest);
extern Quat_t Quat_Slerp(Quat_t a, Quat_t b, float t);


static inline Quat_t Quat_Create(float x, float y, float z, float w)
{
    Quat_t quat;
    quat.x = x;
    quat.y = y;
    quat.z = z;
    quat.w = w;
    return quat;
}

static inline Quat_t Quat_Normalize(Quat_t quaternion)
{
	Quat_t newQuat;
	
	float mag2 = quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z;
	
	if (mag2 != 0.0f && (fabsf(mag2 - 1.0f) > 0.000001))
	{
		float mag = 1.0f / sqrtf(mag2);
		newQuat.w = quaternion.w * mag;
		newQuat.x = quaternion.x * mag;
		newQuat.y = quaternion.y * mag;
		newQuat.z = quaternion.z * mag;
	}
	
	return newQuat;
}

static inline Quat_t Quat_CreateIdentity()
{
    Quat_t quat;
    quat.x = 0.0f;
    quat.y = 0.0f;
    quat.z = 0.0f;
    quat.w = 1.0f;
    return quat;
}

static inline Quat_t Quat_Add(Quat_t a, Quat_t b)
{
    Quat_t vec;
    vec.x = a.x + b.x;
    vec.y = a.y + b.y;
    vec.z = a.z + b.z;
    vec.w = a.w + b.w;
    return vec;
}

static inline Quat_t Quat_Sub(Quat_t a, Quat_t b)
{
    Quat_t vec;
    vec.x = a.x - b.x;
    vec.y = a.y - b.y;
    vec.z = a.z - b.z;
    vec.w = a.w - b.w;
    return vec;
}

static inline Quat_t Quat_Scale(Quat_t quat, float scale)
{
	Quat_t newQuat;
	
	newQuat.x = quat.x * scale;
	newQuat.y = quat.y * scale;
	newQuat.z = quat.z * scale;
	newQuat.w = quat.w * scale;
    
	return newQuat;
}

static inline Quat_t Quat_Negate(Quat_t a)
{
    Quat_t q;
    q.x = -a.x;
    q.y = -a.y;
    q.z = -a.z;
    q.w = a.w;
    return q;
}

static inline Quat_t Quat_Mult(Quat_t a, Quat_t b)
{
    Quat_t vec;
	vec.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	vec.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
	vec.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
	vec.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	return vec;
}

static inline Vec3_t Quat_RotateVec3(Quat_t a, Vec3_t v)
{
	Quat_t vecQuat, resQuat;
    
	vecQuat.x = v.x;
	vecQuat.y = v.y;
	vecQuat.z = v.z;
	vecQuat.w = 0.0f;
    
	resQuat = Quat_Mult(vecQuat, Quat_Negate(a));
	resQuat = Quat_Mult(a, resQuat);
	
	return Vec3_Create(resQuat.x, resQuat.y, resQuat.z);
}

static inline float Quat_Dot(Quat_t a, Quat_t b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


#endif

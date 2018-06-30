
#include "vec_math.h"

void Mat4_Mult(const Mat4_t* a, const Mat4_t* b, Mat4_t* dest)
{
    static Mat4_t temp;
    int i;
    int j;
    int k;
    float n;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            n = 0;
            for (k = 0; k < 4; ++k)
            {
                n += Mat4_Get(b, i, k) * Mat4_Get(a, k, j);
            }
            Mat4_Set(&temp, i, j, n);
        }
    }
    
    Mat4_Copy(dest, &temp);
}

void Mat4_Transpose(Mat4_t* a)
{
    int i;
    int j;
    for (i = 0; i < 4; ++i)
    {
		for (j = i + 1; j < 4; ++j)
        {
			float save = a->m[(i + j * 4)];
			a->m[(i + j * 4)] = a->m[(j + i * 4)];
			a->m[(j + i * 4)] = save;
		}
	}
}

Mat4_t Mat4_CreateLook(Vec3_t eye, Vec3_t target, Vec3_t up)
{
    Vec3_t forward, side, u;
    Mat4_t m;
    Mat4_Identity(&m);
    
    forward = Vec3_Sub(target, eye);
    
    forward = Vec3_Normalize(forward);
    
    side = Vec3_Cross(forward, up);
    side = Vec3_Normalize(side);
    
    u = Vec3_Cross(side, forward);
    
    Mat4_Set(&m, 0, 0, side.x);
    Mat4_Set(&m, 1, 0, side.y);
    Mat4_Set(&m, 2, 0, side.z);
    
    Mat4_Set(&m, 0, 1, u.x);
    Mat4_Set(&m, 1, 1, u.y);
    Mat4_Set(&m, 2, 1, u.z);
    
    Mat4_Set(&m, 0, 2, -forward.x);
    Mat4_Set(&m, 1, 2, -forward.y);
    Mat4_Set(&m, 2, 2, -forward.z);
    
    Mat4_t translate = Mat4_CreateTranslate(Vec3_Negate(eye));
    
    Mat4_Mult(&m, &translate, &m);
    return m;
}

Mat4_t Mat4_CreateFrustum(float left, float right, float bottom, float top, float near, float far)
{
    Mat4_t m;
    Mat4_Identity(&m);
    
    const float invWidth = 1.0f / (right - left);
    const float invHeight = 1.0f / (top - bottom);
    const float invDepth = 1.0f / (far - near);
    
    const float twoZNear = 2.0f * near;
    
    Mat4_Set(&m, 0, 0, twoZNear * invWidth);
    Mat4_Set(&m, 1, 1, twoZNear * invHeight);
    
    Mat4_Set(&m, 2, 0, (right + left) * invWidth);
    Mat4_Set(&m, 2, 1, (top + bottom) * invHeight);
    Mat4_Set(&m, 2, 2, -(far + near) * invDepth);
    Mat4_Set(&m, 2, 3, -1.0f);
    
    Mat4_Set(&m, 3, 2, - twoZNear * far * invDepth);
    return m;
}

float Mat4_Det(const Mat4_t* matrix)
{
    return +Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 0, 3)
    - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 0, 3) + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 0, 3)
    
    + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 0, 3)
    - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 1, 3)
    
    + Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 1, 3)
    - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 1, 3)
    
    + Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 2, 3)
    - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 2, 3) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 2, 3)
    
    + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 2, 3)
    - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 3, 3)
    
    + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 3, 3) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 3, 3)
    - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 3, 3);
}

void Mat4_Inverse(const Mat4_t* matrix, Mat4_t* ret)
{
    /* Yes I actually wrote this from another function not a copy and paste */
    Mat4_Set(ret, 0, 0, +Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 2, 3)
             - Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 1, 0, +Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 2, 3)
             + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 2, 3) + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 3, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 2, 0, +Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 3)
             - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 3, 0, +Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 2) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 2) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 2)
             + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 2) + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 2) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 2));
    
    Mat4_Set(ret, 0, 1, +Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 2, 3)
             + Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 2, 3) + Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 3, 3) - Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 1, 1, +Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 0, 3) + Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 2, 3)
             - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 2, 1, +Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 3)
             + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 3) + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 3) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 3, 1, +Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 2) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 2) + Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 2)
             - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 2, 2) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 2) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 3, 2));
    
    Mat4_Set(ret, 0, 2, +Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 0, 3) + Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 1, 3)
             - Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 1, 2, +Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 1, 3)
             + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 2) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 3, 3) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 2, 2, +Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 3) + Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 3)
             - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 3) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 3));
    
    Mat4_Set(ret, 3, 2, +Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 2) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 0, 2) - Mat4_Get(matrix, 3, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 2)
             + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 3, 1) * Mat4_Get(matrix, 1, 2) + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 3, 2) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 3, 2));
    
    Mat4_Set(ret, 0, 3, +Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 1, 3)
             + Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 2, 3));
    
    Mat4_Set(ret, 1, 3, +Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 0, 3) + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 1, 3)
             - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 2) * Mat4_Get(matrix, 1, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 2) * Mat4_Get(matrix, 2, 3) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 2) * Mat4_Get(matrix, 2, 3));
    
    Mat4_Set(ret, 2, 3, +Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 3) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 3)
             + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 3) + Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 3) - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 3));
    
    Mat4_Set(ret, 3, 3, +Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 0, 2) - Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 0, 2) + Mat4_Get(matrix, 2, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 1, 2)
             - Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 2, 1) * Mat4_Get(matrix, 1, 2) - Mat4_Get(matrix, 1, 0) * Mat4_Get(matrix, 0, 1) * Mat4_Get(matrix, 2, 2) + Mat4_Get(matrix, 0, 0) * Mat4_Get(matrix, 1, 1) * Mat4_Get(matrix, 2, 2));
    
    float det = Mat4_Det(matrix);
    
    int i;
    for (i = 0; i < 16; ++i)
    {
        ret->m[i] = ret->m[i] / det;
    }
}


void Quat_ToMatrix(Quat_t a, Mat4_t* dest)
{
	float x2 = 2.0f * a.x,  y2 = 2.0f * a.y,  z2 = 2.0f * a.z;
	
	float xy = x2 * a.y,  xz = x2 * a.z;
	float yy = y2 * a.y,  yw = y2 * a.w;
	float zw = z2 * a.w,  zz = z2 * a.z;
	
	dest->m[0] = 1.0f - (yy + zz);
	dest->m[1] = (xy - zw);
	dest->m[2] = (xz + yw);
	dest->m[3] = 0.0f;
	
	float xx = x2 * a.x,  xw = x2 * a.w,  yz = y2 * a.z;
	
	dest->m[4] = ( xy +  zw );
	dest->m[5] = 1.0f - ( xx + zz );
	dest->m[6] = ( yz - xw );
	dest->m[7] = 0.0f;
	
	dest->m[8] = ( xz - yw );
	dest->m[9] = ( yz + xw );
	dest->m[10] = 1.0f - ( xx + yy );
	dest->m[11] = 0.0f;
	
	dest->m[12] = 0.0f;
	dest->m[13] = 0.0f;
	dest->m[14] = 0.0f;
	dest->m[15] = 1.0f;
}

Quat_t Quat_FromEuler(float pitch, float yaw, float roll)
{
    Quat_t quat;
    
    float p = pitch * (M_PI / 360.0f);
	float y = yaw * (M_PI / 360.0f);
	float r = roll * (M_PI / 360.0f);
    
	float sinp = sinf(p);
	float siny = sinf(y);
	float sinr = sinf(r);
	float cosp = cosf(p);
	float cosy = cosf(y);
	float cosr = cosf(r);
    
	quat.x = sinr * cosp * cosy - cosr * sinp * siny;
	quat.y = cosr * sinp * cosy + sinr * cosp * siny;
	quat.z = cosr * cosp * siny - sinr * sinp * cosy;
	quat.w = cosr * cosp * cosy + sinr * sinp * siny;
    
    Quat_Normalize(quat);
    
    return quat;
}

Quat_t Quat_FromAngle(float angle, float x, float y, float z)
{
    Quat_t newQuat;
	
	float halfAngle = angle * M_PI / 360.0f;
	
	float sinA = sinf(halfAngle);
	
	newQuat.w = cosf(halfAngle);
	newQuat.x = x * sinA;
	newQuat.y = y * sinA;
	newQuat.z = z * sinA;
	
	return newQuat;
}

Quat_t Quat_Slerp(Quat_t a, Quat_t b, float t)
{
    float w1, w2;
	
	float cosTheta = Quat_Dot(a, b);
	float theta = acosf(cosTheta);
	float sinTheta = sinf(theta);
	
	if (sinTheta > 0.0001f)
	{
		w1 = (sinf((1.0f - t) * theta) / sinTheta);
		w2 = (sinf(t * theta) / sinTheta);
	} else {
		w1 = 1.0f - t;
		w2 = t;
	}
	a = Quat_Scale(a, w1);
	b = Quat_Scale(b, w2);
	
	a = Quat_Add(a, b);
    
	return a;
}

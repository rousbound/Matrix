#ifndef H_MATRIX
#define H_MATRIX
#include <math.h>
#include <vector>

using namespace std;



struct vec3d
{
float x, y, z;
};

struct triangle
{
vec3d p[3];
};

struct mesh
{
vector<triangle> tris;
};

struct mat4x4
{
float m[4][4] = { 0 };
};
//EOF

vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i);
  

mat4x4 Matrix_MakeRotationX(float fAngleRad);
  

  mat4x4 Matrix_MakeRotationY(float fAngleRad);
  

  mat4x4 Matrix_MakeRotationZ(float fAngleRad);
  

mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
  

mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2);
  

mat4x4 Matrix_MakeTranslation(float x, float y, float z);
  

mat4x4 Matrix_MakeIdentity();
  

vec3d Vector_Add(vec3d &v1, vec3d &v2);
  

  vec3d Vector_Sub(vec3d &v1, vec3d &v2);
  

  vec3d Vector_Mul(vec3d &v1, float k);
  

  vec3d Vector_Div(vec3d &v1, float k);
  

float Vector_Length(vec3d &v);
  


float Vector_DotProduct(vec3d &v1, vec3d &v2);
  

vec3d Vector_Normalise(vec3d &v);
  

  vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2);
  
  

mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up);
  

mat4x4 Matrix_QuickInverse(mat4x4 &m); // Only for Rotation/Translation Matrices
  
#endif

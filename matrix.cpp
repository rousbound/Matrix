#include "matrix.h"

vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i)
  {
    vec3d o;
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] +
m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] +
m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] +
m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3
] + m.m[3][3];

    if (w != 0.0f)
    {
      o.x /= w; o.y /= w; o.z /= w;
    }
    return o;
  }



mat4x4 Matrix_MakeRotationX(float fAngleRad)
  {
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
  }

  mat4x4 Matrix_MakeRotationY(float fAngleRad)
  {
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
  }

  mat4x4 Matrix_MakeRotationZ(float fAngleRad)
  {
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
  }

mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
  {
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    mat4x4 matrix;
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / (fFar - fNear);
    matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
  }

mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2)
  {
    mat4x4 matrix;
    for (int c = 0; c < 4; c++)
      for (int r = 0; r < 4; r++)
        matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] +
                         m1.m[r][1] * m2.m[1][c] + 
                         m1.m[r][2] * m2.m[2][c] +
                         m1.m[r][3] * m2.m[3][c];
    return matrix;
  }

mat4x4 Matrix_MakeTranslation(float x, float y, float z)
  {
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
  }

mat4x4 Matrix_MakeIdentity()
  {
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
  }

vec3d Vector_Add(vec3d &v1, vec3d &v2)
  {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
  }

  vec3d Vector_Sub(vec3d &v1, vec3d &v2)
  {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
  }

  vec3d Vector_Mul(vec3d &v1, float k)
  {
    return { v1.x * k, v1.y * k, v1.z * k };
  }

  vec3d Vector_Div(vec3d &v1, float k)
  {
    return { v1.x / k, v1.y / k, v1.z / k };
  }

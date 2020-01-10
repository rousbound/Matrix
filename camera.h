#ifndef CAMERA_H
#define CAMERA_H
#include "matrix.h"
typedef struct cam{
  vec3d acc;
  vec3d vel;
  vec3d pos;
  vec3d rot_acc;
  vec3d rot_vel;
  vec3d rot_pos;
  float impulse;
  float friction;
}Cam;

//EOF

void cameraDynamics();
void debugCamera();
#endif

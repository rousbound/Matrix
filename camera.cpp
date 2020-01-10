#include "camera.h"
#include <stdio.h>


Cam camera = {{0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f},
{0.0f,0.0f,0.0f},
0.0025f, 0.95f};

void cameraDynamics(){
  
  camera.vel = Vector_Mul(camera.vel, camera.friction);
  camera.vel = Vector_Add(camera.vel, camera.acc);
  camera.pos = Vector_Add(camera.pos, camera.vel);

  camera.rot_vel = Vector_Mul(camera.rot_vel, camera.friction);
  camera.rot_vel = Vector_Add(camera.rot_vel, camera.rot_acc);
  camera.rot_pos = Vector_Add(camera.rot_pos, camera.rot_vel);
}

void debugCamera(){
  printf("------------------------------------------------------\n");
  printf("POS: X:,%f,Y:%f,Z:%f\n",camera.pos.x,camera.pos.y,camera.pos.z);
  printf("ACC: X:,%f,Y:%f,Z:%f\n",camera.acc.x,camera.acc.y,camera.acc.z);
  printf("VEL: X:,%f,Y:%f,Z:%f\n",camera.vel.x,camera.vel.y,camera.vel.z);
  printf("ROT_POS: X:,%f,Y:%f,Z:%f\n",camera.rot_pos.x,camera.rot_pos.y,camera.rot_pos.z);
  printf("ROT_ACC: X:,%f,Y:%f,Z:%f\n",camera.rot_acc.x,camera.rot_acc.y,camera.rot_acc.z);
  printf("ROT_VEL: X:,%f,Y:%f,Z:%f\n",camera.rot_vel.x,camera.rot_vel.y,camera.rot_vel.z);
  printf("------------------------------------------------------\n");
}


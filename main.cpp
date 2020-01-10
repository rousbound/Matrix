#include "SDL2/SDL.h"
//#include <fstream>
//#include <strstream>
//#include <algorithm>
#include <algorithm>
#include "FPSlock.h"

#include "matrix.h"

using namespace std;
SDL_Renderer * renderer = NULL;

int WINW = 500;
int WINH = 500;

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

Cam cam = {{0.0f,0.0f,0.0f},
          {0.0f,0.0f,0.0f},
          {0.0f,0.0f,0.0f},
          {0.0f,0.0f,0.0f},
          {0.0f,0.0f,0.0f},
          {0.0f,0.0f,0.0f},
          0.005f, 0.95f};

void camDynamics(){
  
  cam.vel = Vector_Mul(cam.vel, cam.friction);
  cam.vel = Vector_Add(cam.vel, cam.acc);
  cam.pos = Vector_Add(cam.pos, cam.vel);

  cam.rot_vel = Vector_Mul(cam.rot_vel, cam.friction);
  cam.rot_vel = Vector_Add(cam.rot_vel, cam.rot_acc);
  cam.rot_pos = Vector_Add(cam.rot_pos, cam.rot_vel);
}

void lockFPS(){
    frameTime = SDL_GetTicks() - frameStart;

    if(frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);

    }
}


void DrawTriangle(SDL_Renderer* irenderer,int x1, int y1, int x2, int y2, int x3, int y3)
{
  SDL_RenderDrawLine(irenderer,x1,y1,x2,y2);
  SDL_RenderDrawLine(irenderer,x2,y2,x3,y3);
  SDL_RenderDrawLine(irenderer,x3,y3,x1,y1);

}

  mesh meshCube;
  mat4x4 matProj;
  vec3d vCamera;
  vec3d vLookDir;
  float fYaw;

  float fTheta;


bool OnUserCreate() 
{
  meshCube.tris = {

  // SOUTH
  { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
  { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

  // EAST                                                      
  { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

  // NORTH                                                     
  { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
  { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

  // WEST                                                      
  { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
  { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

  // TOP                                                       
  { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
  { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

  // BOTTOM                                                    
  { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
  { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

  };
  

  // Projection Matrix
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 45.0f;
  float fAspectRatio = (float)WINH / (float)WINW;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

  matProj = Matrix_MakeProjection(fFov, fAspectRatio, fNear, fFar);
  

  return true;
}

void debugCamera(){
  printf("------------------------------------------------------\n");
  printf("POS: X:,%f,Y:%f,Z:%f\n",cam.pos.x,cam.pos.y,cam.pos.z); 
  printf("ACC: X:,%f,Y:%f,Z:%f\n",cam.acc.x,cam.acc.y,cam.acc.z); 
  printf("VEL: X:,%f,Y:%f,Z:%f\n",cam.vel.x,cam.vel.y,cam.vel.z); 
  printf("ROT_POS: X:,%f,Y:%f,Z:%f\n",cam.rot_pos.x,cam.rot_pos.y,cam.rot_pos.z); 
  printf("ROT_ACC: X:,%f,Y:%f,Z:%f\n",cam.rot_acc.x,cam.rot_acc.y,cam.rot_acc.z); 
  printf("ROT_VEL: X:,%f,Y:%f,Z:%f\n",cam.rot_vel.x,cam.rot_vel.y,cam.rot_vel.z); 
  printf("------------------------------------------------------\n");
}
bool OnUserUpdate(float deltaTime)
{
  mat4x4 matRotZ, matRotX, matRotY;
  mat4x4 matTrans, matTrans2;
  fTheta += 1.0f * deltaTime;
  camDynamics();
  debugCamera();

  mat4x4 matWorld;
  
  vec3d vUp = {0,1,0};
  vec3d vTarget = {0,0,1};
  //mat4x4 matCamera = Matrix_MakeRotationY(1.0f* fTheta);

  mat4x4 matCamera;
  mat4x4 camRotX, camRotY, camRotZ;
  
  matCamera = Matrix_MakeIdentity();

  camRotX = Matrix_MakeRotationX(cam.rot_pos.x);
  camRotY = Matrix_MakeRotationY(cam.rot_pos.y);
  camRotZ = Matrix_MakeRotationZ(cam.rot_pos.z);


  vLookDir = Matrix_MultiplyVector(matCamera,vTarget);
  vTarget = Vector_Add(vCamera,vLookDir);
  matCamera = Matrix_PointAt(cam.pos, vTarget, vUp);

  matCamera = Matrix_MultiplyMatrix(matCamera,camRotX);
  matCamera = Matrix_MultiplyMatrix(matCamera,camRotY);
  matCamera = Matrix_MultiplyMatrix(matCamera,camRotZ);

  matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
  matRotX = Matrix_MakeRotationX(fTheta);
  matRotY = Matrix_MakeRotationY(fTheta);


  matTrans = Matrix_MakeTranslation(0.0f+cam.pos.x, 0.0f, 5.0f-cam.pos.z);
  matTrans2 = Matrix_MakeTranslation(-0.5f, -0.5f, -0.5f);

  
  matWorld = Matrix_MakeIdentity();
  matWorld = Matrix_MultiplyMatrix(matWorld, matTrans2);
  matWorld = Matrix_MultiplyMatrix(matWorld, matRotX);
  matWorld = Matrix_MultiplyMatrix(matWorld, matRotY);
  matWorld = Matrix_MultiplyMatrix(matWorld, matRotZ);
  matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);


  mat4x4 matView = Matrix_QuickInverse(matCamera);
  
  vector<triangle> vecTrianglesToRaster;

  // Draw Triangles
  for (auto tri : meshCube.tris)
  {
    triangle triProjected, triTranslated, triTransformed, triViewed;

    triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
    triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
    triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);


    vec3d normal, line1, line2;

    line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
    line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

    normal = Vector_CrossProduct(line1, line2);

    normal = Vector_Normalise(normal);
    
    vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);

    if (Vector_DotProduct(normal, vCameraRay) < 0.0f) {


    triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
    triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
    triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);

    // Project triangles from 3D --> 2D
    triProjected.p[0] = Matrix_MultiplyVector(matProj, triViewed.p[0]);
    triProjected.p[1] = Matrix_MultiplyVector(matProj, triViewed.p[1]);
    triProjected.p[2] = Matrix_MultiplyVector(matProj, triViewed.p[2]);


    // Scale into view
    triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
    triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
    triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
    triProjected.p[0].x *= 0.5f * (float)WINW;
    triProjected.p[0].y *= 0.5f * (float)WINH;
    triProjected.p[1].x *= 0.5f * (float)WINW;
    triProjected.p[1].y *= 0.5f * (float)WINH;
    triProjected.p[2].x *= 0.5f * (float)WINW;
    triProjected.p[2].y *= 0.5f * (float)WINH;

    vecTrianglesToRaster.push_back(triProjected);

    
    }

  sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2)
    {
      float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
      float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
      return z1 > z2;
    });
  
  for (auto &triProjected : vecTrianglesToRaster)
    {
        DrawTriangle(renderer,triProjected.p[0].x, triProjected.p[0].y,
        triProjected.p[1].x, triProjected.p[1].y,
        triProjected.p[2].x, triProjected.p[2].y);
  
    }

  }
  
  

  return true;
}



int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;

        if (SDL_CreateWindowAndRenderer(WINW, WINH, 0, &window
  , &renderer) == 0) {
            SDL_bool done = SDL_FALSE;
            OnUserCreate();

            SDL_Event event;
            while (!done) {
              frameStart = SDL_GetTicks();
              SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
              SDL_RenderClear(renderer);
              SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
              OnUserUpdate(0.0125f);
              SDL_RenderPresent(renderer);

              while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                      done = SDL_TRUE;
                  }
                    if (event.type == SDL_KEYDOWN){
                        if (event.key.keysym.sym == SDLK_w){
                          cam.acc.z = cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_s){
                          cam.acc.z = -cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_a){
                          cam.acc.x = cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_d){
                          cam.acc.x = -cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_UP){
                          cam.rot_acc.x = cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_DOWN){
                          cam.rot_acc.x = -cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_LEFT){
                          cam.rot_acc.y = cam.impulse;
                        }
                        if (event.key.keysym.sym == SDLK_RIGHT){
                          cam.rot_acc.y = -cam.impulse;
                        }
                    }
                    if (event.type == SDL_KEYUP){
                        if (event.key.keysym.sym == SDLK_w){
                          if (cam.acc.z > 0)
                            cam.acc.z = 0;
                        }
                        if (event.key.keysym.sym == SDLK_s){
                          if (cam.acc.z < 0)
                            cam.acc.z = 0;
                        }
                        if (event.key.keysym.sym == SDLK_a){
                          if (cam.acc.x > 0)
                            cam.acc.x = 0;
                        }
                        if (event.key.keysym.sym == SDLK_d){
                          if (cam.acc.x < 0)
                            cam.acc.x = 0;
                        }
                        if (event.key.keysym.sym == SDLK_UP){
                          if (cam.rot_acc.x > 0)
                            cam.rot_acc.x = 0;
                          cam.rot_acc.x = 0;
                        }
                        if (event.key.keysym.sym == SDLK_DOWN){
                          if (cam.rot_acc.x < 0)
                            cam.rot_acc.x = 0;
                          cam.rot_acc.x = 0;
                        }
                        if (event.key.keysym.sym == SDLK_LEFT){
                          if (cam.rot_acc.y > 0)
                            cam.rot_acc.y = 0;
                          cam.rot_acc.y = 0;
                        }
                        if (event.key.keysym.sym == SDLK_RIGHT){
                          if (cam.rot_acc.y < 0)
                            cam.rot_acc.y = 0;
                          cam.rot_acc.y = 0;
                        }
                    }

                }
              lockFPS();
            }

            if (renderer) {
                SDL_DestroyRenderer(renderer);
            }
            if (window) {
                SDL_DestroyWindow(window);
            }
          }
            SDL_Quit();
            return 0;
            }
      return 0;
}

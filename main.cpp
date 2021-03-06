#include "SDL2/SDL.h"
//#include <fstream>
//#include <strstream>
//#include <algorithm>
#include <algorithm>
#include <stdlib.h>
#include "FPSlock.h"

#include "matrix.h"
#include "camera.h"





extern Cam camera;

using namespace std;
SDL_Renderer * renderer = NULL;
SDL_Window   *   window = NULL;

int WINW = 500;
int WINH = 500;

float deltaTime = 0.0125f;

typedef struct mouse{
  int x;
  int y;
} Mouse;

Mouse cursor = {WINW/2, WINH/2};

void handleMouse(int * deltax, int * deltay){
    printf("MOUSE POS X:%d, Y:%d\n",cursor.x,cursor.y);
    printf("MOUSE DELTA X:%d, Y:%d\n",*deltax,*deltay);
    if(*deltay > 0)
      camera.rot_acc.x = -camera.impulse;
    else if(*deltay < 0)
      camera.rot_acc.x = camera.impulse;
    else
      camera.rot_acc.x = 0;
      if(*deltax > 0)
      camera.rot_acc.y = -camera.impulse;
    else if(*deltax < 0)
      camera.rot_acc.y = camera.impulse;
    else
      camera.rot_acc.y = 0;
    *deltax = 0;
    *deltay = 0;
    
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

bool OnUserUpdate(float deltaTime)
{
  mat4x4 matRotZ, matRotX, matRotY;
  mat4x4 matTrans, matTrans2;
  fTheta += 1.0f * deltaTime;
  cameraDynamics();
  //debugCamera();

  mat4x4 matWorld;
  
  vec3d vUp = {0,1,0};
  vec3d vTarget = {0,0,1};
  //mat4x4 matCamera = Matrix_MakeRotationY(1.0f* fTheta);

  mat4x4 matCamera;
  mat4x4 cameraRotX, cameraRotY, cameraRotZ;
  
  matCamera = Matrix_MakeIdentity();

  cameraRotX = Matrix_MakeRotationX(camera.rot_pos.x);
  cameraRotY = Matrix_MakeRotationY(camera.rot_pos.y);
  cameraRotZ = Matrix_MakeRotationZ(camera.rot_pos.z);

  matCamera = Matrix_MultiplyMatrix(matCamera,cameraRotX);
  matCamera = Matrix_MultiplyMatrix(matCamera,cameraRotY);
  matCamera = Matrix_MultiplyMatrix(matCamera,cameraRotZ);

  vLookDir = Matrix_MultiplyVector(matCamera,vTarget);
  vTarget = Vector_Add(camera.pos,vLookDir);
  matCamera = Matrix_PointAt(camera.pos, vTarget, vUp);


  matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
  matRotX = Matrix_MakeRotationX(fTheta);
  matRotY = Matrix_MakeRotationY(fTheta);


  matTrans = Matrix_MakeTranslation(0.0f+camera.pos.x, 0.0f, 5.0f-camera.pos.z);
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

void handleKeyboard(SDL_Event event){

    if (event.type == SDL_KEYDOWN){
      if (event.key.keysym.sym == SDLK_w){
        camera.acc.z = camera.impulse;
      }
      if (event.key.keysym.sym == SDLK_s){
        camera.acc.z = -camera.impulse;
      }
      if (event.key.keysym.sym == SDLK_a){
        camera.acc.x = camera.impulse;
      }
      if (event.key.keysym.sym == SDLK_d){
        camera.acc.x = -camera.impulse;
      }
     /* if (event.key.keysym.sym == SDLK_UP){
        camera.rot_acc.x = camera.impulse;
      }
      if (event.key.keysym.sym == SDLK_DOWN){
        camera.rot_acc.x = -camera.impulse;
      }
      if (event.key.keysym.sym == SDLK_LEFT){
        camera.rot_acc.y = camera.impulse;
      }
      if (event.key.keysym.sym == SDLK_RIGHT){
        camera.rot_acc.y = -camera.impulse;
      }*/
  }
  if (event.type == SDL_KEYUP){
      if (event.key.keysym.sym == SDLK_w){
        if (camera.acc.z > 0)
          camera.acc.z = 0;
      }
      if (event.key.keysym.sym == SDLK_s){
        if (camera.acc.z < 0)
          camera.acc.z = 0;
      }
      if (event.key.keysym.sym == SDLK_a){
        if (camera.acc.x > 0)
          camera.acc.x = 0;
      }
      if (event.key.keysym.sym == SDLK_d){
        if (camera.acc.x < 0)
          camera.acc.x = 0;
      }
      /*if (event.key.keysym.sym == SDLK_UP){
        if (camera.rot_acc.x > 0)
          camera.rot_acc.x = 0;
        camera.rot_acc.x = 0;
      }
      if (event.key.keysym.sym == SDLK_DOWN){
        if (camera.rot_acc.x < 0)
          camera.rot_acc.x = 0;
        camera.rot_acc.x = 0;
      }
      if (event.key.keysym.sym == SDLK_LEFT){
        if (camera.rot_acc.y > 0)
          camera.rot_acc.y = 0;
        camera.rot_acc.y = 0;
      }
      if (event.key.keysym.sym == SDLK_RIGHT){
        if (camera.rot_acc.y < 0)
          camera.rot_acc.y = 0;
        camera.rot_acc.y = 0;
      }*/
  }
}



int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {

        if (SDL_CreateWindowAndRenderer(WINW, WINH, 0, &window
  , &renderer) == 0) {
            SDL_bool done = SDL_FALSE;
            SDL_SetRelativeMouseMode(SDL_TRUE);
            OnUserCreate();
            int deltax = 0;
            int deltay = 0;
            while (!done) {
              frameStart = SDL_GetTicks();
              SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
              SDL_RenderClear(renderer);
              SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
              OnUserUpdate(deltaTime);
              SDL_RenderPresent(renderer);




              SDL_Event event;
              while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                      done = SDL_TRUE;
                  }
                  else if (event.type == SDL_MOUSEMOTION){
                    deltax = event.motion.xrel; 
                    deltay = event.motion.yrel;
                  }
                  handleKeyboard(event);

                }
              handleMouse(&deltax,&deltay);
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

#include <SDL2/SDL.h>
//#include <fstream>
//#include <strstream>
//#include <algorithm>
#include "FPSlock.h"

#include "matrix.h"

using namespace std;
SDL_Renderer * renderer = NULL;

int WINW = 500;
int WINH = 500;

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

  mat4x4 matWorld;


  matRotZ = Matrix_MakeRotationZ(fTheta * 0.5f);
  matRotX = Matrix_MakeRotationX(fTheta);
  matRotY = Matrix_MakeRotationY(fTheta);
  matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);
  matTrans2 = Matrix_MakeTranslation(-0.5f, -0.5f, -0.5f);

  
  matWorld = Matrix_MakeIdentity();
  matWorld = Matrix_MultiplyMatrix(matWorld, matTrans2);
  matWorld = Matrix_MultiplyMatrix(matWorld, matRotX);
  matWorld = Matrix_MultiplyMatrix(matWorld, matRotY);
  matWorld = Matrix_MultiplyMatrix(matWorld, matRotZ);
  matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);
  

  // Draw Triangles
  for (auto tri : meshCube.tris)
  {
    triangle triProjected, triTranslated, triTransformed;

    triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
    triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
    triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);

    // Project triangles from 3D --> 2D
    triProjected.p[0] = Matrix_MultiplyVector(matProj, triTransformed.p[0]);
    triProjected.p[1] = Matrix_MultiplyVector(matProj, triTransformed.p[1]);
    triProjected.p[2] = Matrix_MultiplyVector(matProj, triTransformed.p[2]);

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

    // Rasterize triangle
    DrawTriangle(renderer,triProjected.p[0].x, triProjected.p[0].y,
      triProjected.p[1].x, triProjected.p[1].y,
      triProjected.p[2].x, triProjected.p[2].y);

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
              printf("Running!\n");
              SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
              SDL_RenderClear(renderer);
              SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
              OnUserUpdate(0.025f);
              SDL_RenderPresent(renderer);

              while (SDL_PollEvent(&event)) {
                  if (event.type == SDL_QUIT) {
                      done = SDL_TRUE;
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

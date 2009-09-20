/*                                                              */
/* This test is based on Mark J. Kilgard's molehill example     */
/*                                                              */
/* // Mike Gorchak, 2009. GLU ES test                           */
/*                                                              */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengles.h>

#define __USE_SDL_GLES__
#include "glues.h"

/* screen width, height, and bit depth */
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

GLfloat mat_ambient[4]={0.2f, 0.2f, 0.2f, 0.2f};
GLfloat mat_diffuse[]={0.7f, 0.7f, 0.7f, 1.0f};
GLfloat mat_specular[]={1.0f, 1.0f, 1.0f, 1.0f};
GLfloat mat_shininess[]={100.0f};

GLfloat ctlpoints[4][4][3];

GLfloat knots[8]={0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
/* counter clockwise */
GLfloat edgePt[5][2]={{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}};
/* clockwise */
GLfloat curvePt[4][2]={{0.25f, 0.5f}, {0.25f, 0.75f}, {0.75f, 0.75f}, {0.75f, 0.5f}};
GLfloat curveKnots[8]={0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
/* clockwise */
GLfloat pwlPt[4][2]={{0.75f, 0.5f}, {0.5f, 0.25f}, {0.25f, 0.5f}};

GLfloat zrot=3.0f;

GLUnurbsObj* nurb;

void init_scene(int width, int height)
{
   int u, v;

   /* Setup our viewport for OpenGL ES */
   glViewport(0, 0, (GLint)width, (GLint)height);
   /* Setup our viewport for GLU ES (required when using OpenGL ES 1.0 only) */
   gluViewport(0, 0, (GLint)width, (GLint)height);

   /* Set black background color */
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);

   /* Since GL ES has no GL_AUTO_NORMAL, we implement it in GLU */
   gluEnable(GLU_AUTO_NORMAL);

   nurb=gluNewNurbsRenderer();
   gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 150.0f);
   gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);

   for (u=0; u<4; u++)
   {
      for (v=0; v<4; v++)
      {
         ctlpoints[u][v][0]=2.0*((GLfloat)u-1.5);
         ctlpoints[u][v][1]=2.0*((GLfloat)v-1.5);

         if ((u==1 || u==2) && (v==1 || v==2))
         {
            ctlpoints[u][v][2]=3.0;
         }
         else
         {
            ctlpoints[u][v][2]=-3.0;
         }
      }
   }

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT, 3.0f, 8.0f);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0f, 0.5f, -5.5f);
   glRotatef(330.0f, 1.0f, 0.0f, 0.0f);
   glScalef(0.5f, 0.5f, 0.5f);
}

void render_scene()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glRotatef(zrot, 0.0f, 0.0f, 1.0f);

   /* Render trimmed surface */
   gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots, 4*3, 3, &ctlpoints[0][0][0], 4, 4, GLU_MAP2_VERTEX_3);
      gluBeginTrim(nurb);
         gluPwlCurve(nurb, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
      gluEndTrim(nurb);
      gluBeginTrim(nurb);
         gluNurbsCurve(nurb, 8, curveKnots, 2, &curvePt[0][0], 4, GLU_MAP1_TRIM_2);
         gluPwlCurve (nurb, 3, &pwlPt[0][0], 2, GLU_MAP1_TRIM_2);
      gluEndTrim(nurb);
   gluEndSurface(nurb);

   /* Flush all drawings */
   glFlush();
}

int main(int argc, char** argv)
{
   int status;
   SDL_WindowID window;
   SDL_GLContext glcontext=NULL;
   SDL_Event event;
   SDL_bool done=SDL_FALSE;

   status=SDL_Init(SDL_INIT_VIDEO);
   if (status<0)
   {
      fprintf(stderr, "Can't init default SDL video driver: %s\n", SDL_GetError());
      exit(-1);
   }

   /* Select first display */
   status=SDL_SelectVideoDisplay(0);
   if (status<0)
   {
      fprintf(stderr, "Can't attach to first display: %s\n", SDL_GetError());
      exit(-1);
   }

   window=SDL_CreateWindow("SDL GLU ES Nurbs Trim test",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
   if (window==0)
   {
      fprintf(stderr, "Can't create window: %s\n", SDL_GetError());
      exit(-1);
   }

   glcontext=SDL_GL_CreateContext(window);
   if (glcontext==NULL)
   {
      fprintf(stderr, "Can't create OpenGL ES context: %s\n", SDL_GetError());
      exit(-1);
   }

   status=SDL_GL_MakeCurrent(window, glcontext);
   if (status<0)
   {
      fprintf(stderr, "Can't set current OpenGL ES context: %s\n", SDL_GetError());
      exit(-1);
   }

   init_scene(WINDOW_WIDTH, WINDOW_HEIGHT);

   do {
      /* handle the events in the queue */
      while (SDL_PollEvent(&event))
      {
         switch(event.type)
         {
            case SDL_WINDOWEVENT:
                 switch (event.window.event)
                 {
                    case SDL_WINDOWEVENT_CLOSE:
                         done=SDL_TRUE;
                         break;
                 }
                 break;
            case SDL_KEYDOWN:
                 switch (event.key.keysym.sym)
                 {
                    case SDLK_ESCAPE:
                         done=SDL_TRUE;
                         break;
                 }
                 break;
            case SDL_QUIT:
                 done=SDL_TRUE;
                 break;
         }
      }

      if (done==SDL_TRUE)
      {
         break;
      }

      render_scene();
      SDL_GL_SwapWindow(window);
   } while(1);

   SDL_GL_DeleteContext(glcontext);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}
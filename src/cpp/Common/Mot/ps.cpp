
/* Simple program:  Fill a colormap with gray and stripe it down the screen */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;

#include "SDL.h"
#include "basic3d.h"
#include "ParticleBase.h"
#include "ParticleEmitter.h"
#include "ParticleCarrier.h"
#include "ParticleDistribution.h"
#include "ParticleGroup.h"
#include "MotDistributionPos.h"
#include "MotDistributionVector.h"

CParticleGroup* PSRoot;

void InitParticles()
{    
  PSRoot = new CParticleGroup();
  
  PSRoot->SetX(0.0);
  PSRoot->SetY(0.0);

  CDistributionColor* lDC;
  CParticleEmitter*   lPG;
  CParticleCarrier*   lPC;
    
  lDC = new CDistributionColor(
    new CDistributionRandom(0, 100), 
    new CDistributionRandom(150, 200),
    //new CDistributionConstant(0.0),
    new CDistributionRandom(220, 255)
    //new CDistributionConstant(0.0)
    );
  
  //TPos3D lDistributionPos1 = {0., 0., 0.};    
  //lPosD = new CMotDistributionEllipse(100., 50., lDistributionPos1);      
  
  TPos3D lDistributionPos1 = {-5., 0., 0.};    
  TPos3D lDistributionPos2 = {5., 0., 0.};    
  
  /*  
  lPG = new CParticleEmitter(
      100,
      new CDistributionRandom(2, 5), // life distribution 
      lDC, 
      //new CMotDistributionSegment(lDistributionPos1, lDistributionPos2),
      new CMotDistributionDisc(180., 180., lDistributionPos1),
      //new CMotDistributionVectorCone(CVector3D(0., -1., 0.), 90., 80., 160.)
      new CMotDistributionVectorConstant(CVector3D(1., 1., -1.), 15, 25)
    );
  
  lPG->AddAction(new CMotActionMatchColor(255, 255, 255, 5));  
  //lPG->AddAction(new CMotActionFollow(15., 100.)); 
  
  lPG->AddAction(new CMotActionAccelerate(CVector3D(0., 7., 0.)));  
  
  //TPos3D lPos1 = {0., -50., 0.}; 
  //TPos3D lPos2 = {100, 30, 30}; 
  //lPG->AddAction(new CMotActionOrbitPoint(20., 400., lPos1));  
  //lPG->AddAction(new CMotActionOrbitPoint(20., 80., lPos2));  
  
    
  PSRoot->Add(lPG);
  */
  
  // a walking line generator
  lDistributionPos1.x = 0.; lDistributionPos1.y = -13; lDistributionPos1.z = 0;
  lDistributionPos2.x = 0.; lDistributionPos2.y = 13; lDistributionPos2.z = 0;
  
  lPG = new CParticleEmitter(
      50,
      new CDistributionRandom(2, 5), // life distribution 
      lDC, 
      //new CMotDistributionSegment(lDistributionPos1, lDistributionPos2),
      new CMotDistributionSegment(lDistributionPos1, lDistributionPos2),
      //new CMotDistributionVectorCone(CVector3D(0., -1., 0.), 90., 80., 160.)
      new CMotDistributionVectorConstant(CVector3D(-1., 0., 0.), 25, 55)
    );
    
  lPG->AddAction(new CMotActionMatchColor(255, 255, 255, 5));  
  
  PSRoot->Add(lPG);
  
      
  //CMotTrajectory* lT = new CMotTrajectoryCircle(60.0);
  //lT->SetTime(-2.3445);
  
  //lPC = new CParticleCarrier(lPG, lT);
  //lPC->SetX(-50.); lPC->SetY(-50.);
  
  //PSRoot->Add(lPC);
  
    
  //lPG = new CParticleEmitter(100);
  //lPG->SetX(-50.); lPG->SetY(-50.);
  //PSRoot.push_back(lPG);
  
  /*
  CDistributionColor* lDC = new CDistributionColor(
    new CDistributionRandom(200, 255), 
    new CDistributionRandom(200, 255),
    new CDistributionConstant(255.0)
    );
    
  CParticleEmitter* lPG = new CParticleEmitter(50, lDC);
    
  PSRoot->Add(lPG);
  
  CParticleCarrier* lPC = new CParticleCarrier(lPG, new CMotTrajectorySin(40.0));
  lPC->SetX(50.); lPC->SetY(50.);
  PSRoot->Add(lPC);  
  */
           
}

void DrawBackground(SDL_Surface *screen)
{
	int *buffer;

	SDL_FillRect(screen, 0, 0);
	
	TPos2D lOrigin = { 320, 240 };
	
	PSRoot->Update();
	PSRoot->Paint(screen, lOrigin);
	
	if ( screen->flags & SDL_DOUBLEBUF ) 
	{
	  SDL_Flip(screen);
	} 
	else 
	{
	  SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
}

SDL_Surface *CreateScreen(Uint16 w, Uint16 h, Uint8 bpp, Uint32 flags)
{
	SDL_Surface *screen;
	int i;

	/* Set the video mode */
	screen = SDL_SetVideoMode(w, h, bpp, flags);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set display mode: %s\n",
							SDL_GetError());
		return(NULL);
	}
	fprintf(stderr, "Screen is in %s mode\n",
		(screen->flags & SDL_FULLSCREEN) ? "fullscreen" : "windowed");

	return(screen);
}


int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	Uint32 videoflags;
	int    done;
	SDL_Event event;
	int width, height, bpp;

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}

	/* See if we try to get a hardware colormap */
	width = 640;
	height = 480;
	bpp = 32;
	videoflags = SDL_SWSURFACE;

	/* Set a video mode */
	screen = CreateScreen(width, height, bpp, videoflags);
	if ( screen == NULL ) 
	{
		exit(2);
	}
        
        //DrawBackground(screen);
	
	InitParticles();
		
	/* Wait for a keystroke */
	done = 0;
	while ( !done) 
	{
	        DrawBackground(screen);
	  
	        SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN:
				/* Ignore ALT-TAB for windows */
				if ( (event.key.keysym.sym == SDLK_LALT) ||
				     (event.key.keysym.sym == SDLK_TAB) ) {
					break;
				}
				/* Center the mouse on <SPACE> */
				if ( event.key.keysym.sym == SDLK_SPACE ) {
					SDL_WarpMouse(width/2, height/2);
					break;
				}
				/* Toggle fullscreen mode on <RETURN> */
				if ( event.key.keysym.sym == SDLK_RETURN ) {
					videoflags ^= SDL_FULLSCREEN;
					screen = CreateScreen(
						screen->w, screen->h,
						screen->format->BitsPerPixel,
								videoflags);
					if ( screen == NULL ) {
						fprintf(stderr,
					"Couldn't toggle fullscreen mode\n");
						done = 1;
					}
                                        DrawBackground(screen);
					break;
				}
				
				if (event.key.keysym.sym == SDLK_x)
				{
				  done = 1;
				}
				
				/* Any other key quits the application... */
			case SDL_QUIT:
				done = 1;
				break;
			default:
				break;
		}
	}
	SDL_Quit();
	return(0);
}


/* Simple program:  Fill a colormap with gray and stripe it down the screen */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;

#include "SDL.h"
#include "SDL_main.h"
#include "HeightMap.h"
#include "TGenHillAlg.h"
#include "TGenFluidAlg.h"
#include "TGenFaultAlg.h"
#include "TGenScriptAlg.h"

HeightMap* gMap, *gMap2;

#pragma comment(lib, "C:/Progra~1/Borland/CBuilder6/Lib/SDLmain.lib")
#pragma comment(lib, "C:/Progra~1/Borland/CBuilder6/Lib/SDL.lib")

void InitTGen()
{   
  cout << "generation started" << endl;
  
  TGenHillAlg lAlg1;
  
  lAlg1.SetWidth(256);
  lAlg1.SetHeight(256);
  lAlg1.SetMinRadius(30.);
  lAlg1.SetMaxRadius(50.);
  lAlg1.SetHills(200);
  
  gMap = lAlg1.Generate();
  
  //gMap->Normalize();

  /*
  TGenFluidAlg lAlg;
    
  lAlg.SetMinHeight(-500.);
  lAlg.SetMaxHeight(500.);
  
  lAlg.SetC(50);
  lAlg.SetD(10);
  lAlg.SetT(0.033);
  lAlg.SetMu(100);
  
  lAlg.SetPasses(100);
  */

  /*
  TGenFaultAlg lAlg2;

  lAlg2.SetPasses(200);
  lAlg2.SetDisplacement(5.);

  lAlg2.SetWidth(256);
  lAlg2.SetHeight(256);
  lAlg2.SetInputMap(gMap);

  gMap2 = lAlg2.Generate();

  gMap2->Normalize();
  */

  TGenScriptAlg lAlg3;

  lAlg3.SetWidth(256);
  lAlg3.SetHeight(256);

  lAlg3.SetScriptFromFile("./gen1.tgen");
  lAlg3.SetInputMap(gMap);
  gMap2 = lAlg3.Generate();

  gMap2->Normalize();
  gMap->Normalize();

  //cout << (char*)(lAlg3.GetScript()) << endl;

  //gMap->Scale(64., 255.);
  cout << "generation finished" << endl;
}

void PaintMap(SDL_Surface *screen, HeightMap* _map, int _x, int _y)
{
	for(int x = 0; x < _map->GetWidth(); x++)
	{
	  for(int y = 0; y < _map->GetHeight(); y++)
	  {
	    //if (_map->Get(x, y) > 0.5)// && _map->Get(x, y) < 0.6)
	    {
	    int lV = (int)(_map->Get(x, y) * 255);
	    //intt lV = 255;
	    //cout << lV << " ";
	    
	    int lColor = (lV << 16) | (lV << 8) | lV;
	    
	    SDL_Rect lRect;
	    
	    lRect.x = x + _x;
	    lRect.y = y + _y;
	    lRect.w = 1;
	    lRect.h = 1;
	    
	    SDL_FillRect(screen, &lRect, lColor); 	    
	    }
	  }
	}
}

void DrawBackground(SDL_Surface *screen)
{
	int *buffer;

	SDL_FillRect(screen, 0, 0);
	
	PaintMap(screen, gMap, 0, 0);
	
	//gMap2->Smooth(0.5);
	
	PaintMap(screen, gMap2, 256, 0);
	
				
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

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[])
//void main()
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

        srand(SDL_GetTicks());
        
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
	
	InitTGen();
        DrawBackground(screen);
		
	/* Wait for a keystroke */
	done = 0;
	while ( !done) 
	{
	  
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

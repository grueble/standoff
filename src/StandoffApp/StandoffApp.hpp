#ifndef _STANDOFF_APP_HPP_
#define _STANDOFF_APP_HPP_

#include <SDL.h>
#undef _main
#include "Game/Game.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace StandoffApp_n
{
   //Screen dimension constants
   static const int SCREEN_WIDTH = 600;
   static const int SCREEN_HEIGHT = 840;

   // the board's upper left hand corner
   static const std::pair<int,int> BOARD_COORD = std::pair(60, 240); 
   
   // the width of (both) the border tiles (and the margin)
   static const int TILE_WIDTH = 60; 

   class StandoffApp_c
   {
   public:
      // constructor
      StandoffApp_c();

      // default destructor
      ~StandoffApp();

      // \Name: init
      // \Description:
      // - initializes the StandoffApp's SDL2 functionality
      // \Argument:
      // - none
      // \Returns
      // - bool, success of failure
      bool init();

      // \Name: loadMedia
      // \Description:
      // - initializes the resource manager
      // \Argument:
      // - none
      // \Returns
      // - bool, success or failure
      bool loadMedia();

      // \Name: run
      // \Description:
      // - runs the Standoff (client) Application
      // \Argument:
      // - none
      // \Returns
      // - none
      bool run();

      // \Name: close
      // \Description:
      // - closes the Standoff (client) Application
      // \Argument:
      // - none
      // \Returns
      // - none
      void close();

   protected:
      // the window to render to
      SDL_Window* gWindow = NULL;

      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // stores the client's game instance
      // -> can be changed to a container when multiple ongoing games are implemented
      Game_c* mGame = NULL;

      // bulk loads assets and manages them for the client
      ResourceManager_c* mResourceManager;

      // the cursor's on screen location
      SDL_Rect* mCursor = NULL;
   };
}

#endif _STANDOFF_APP_HPP_
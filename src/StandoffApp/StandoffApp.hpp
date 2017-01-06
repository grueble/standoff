#ifndef _STANDOFF_APP_HPP_
#define _STANDOFF_APP_HPP_

#include <SDL.h>
#include <utility>
#include "Game/Game.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace StandoffApp_n
{
   // the width of (both) the border tiles (and the margin)
   static const int TILE_WIDTH = 60; 

   /* 
    * NOTE: to transform a "screen tile" point into a window 
    * point (in pixels), multiply the coordinate's  x and y 
    * values by TILE_WIDTH
    */

   // screen dimension constants
   static const int SCREEN_WIDTH = TILE_WIDTH * 11;
   static const int SCREEN_HEIGHT = TILE_WIDTH * 17;

   // the board's upper left hand corner in "screen tile" coordinates
   // -> the window's upper left hand corner is at (0, 0)
   static const std::pair<int,int> BOARD_COORD = std::make_pair(1, 4); 

   // the board's side length
   static const int BOARD_SIDE_LENGTH = 9;

   class StandoffApp_c
   {
   public:
      // constructor
      StandoffApp_c();

      // default destructor
      ~StandoffApp_c();

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
      int run();

      // \Name: close
      // \Description:
      // - closes the Standoff (client) Application
      // \Argument:
      // - none
      // \Returns
      // - none
      void close();

   protected:
      // \Name: handleLmbDown
      // \Description:
      // - handles an SDL_MOUSEBUTTONDOWN event (only for SDL_BUTTON_LEFT)
      // \Argument:
      // - const SDL_Event&, the event to handle 
      // - Game_c&, the active game
      // - Player_c&, address of the current player
      // - Piece_c*, pointer to the current piece (can be NULL)
      // \Returns
      // - void
      void handleLmbDown(const SDL_Event& e, Game_n::Game_c& current_game);

      // \Name: handleKeyDown
      // \Description:
      // - handles an SDL_KEYDOWN event 
      // \Argument:
      // - const SDL_Event&, the event to handle 
      // - Game_c&, the active game
      // - Player_c&, address of the current player
      // - Piece_c*, pointer to the current piece (can be NULL)
      // \Returns
      // - void
      void handleKeyDown(const SDL_Event& e,
                         Game_n::Game_c& current_game);

      // \Name: getImage
      // \Description:
      // - gets an image of the parameterized ImageType_e
      // \Argument:
      // - ImageType_e, the image to get
      // \Returns
      // - SDL_Texture*, pointer to the desired texture
      SDL_Texture* getImage(ResourceManager_n::ImageType_e image_type);

      // the window to render to
      SDL_Window* gWindow = NULL;

      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // the current displayed texture
      SDL_Texture* gTexture = NULL;

      // bulk loads assets and manages them for the client
      ResourceManager_n::ResourceManager_c* mResourceManager = NULL;
   };
}

#endif _STANDOFF_APP_HPP_
#ifndef _STANDOFF_APP_HPP_
#define _STANDOFF_APP_HPP_

#include <SDL.h>
#include "Game/Game.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace StandoffApp_n
{
   enum DrawType_e
   {
      START = 0,
      MOVE = 1,
      UNDO_MOVE = 2,
      SHOOTOUT = 3
   };

   class StandoffApp_c
   {
   public:
      // constructor
      StandoffApp_c(ResourceManager_n::ResourceManager_c& resource_manager);

      // default destructor
      ~StandoffApp_c();

      // \Name: run
      // \Description:
      // - runs the Standoff (client) Application
      // \Argument:
      // - none
      // \Returns
      // - none
      int run();

   protected:
      // \Name: handleLmbDown
      // \Description:
      // - handles an SDL_MOUSEBUTTONDOWN event (only for SDL_BUTTON_LEFT)
      // \Argument:
      // - const SDL_Event&, the event to handle 
      // \Returns
      // - void
      void handleLmbDown(const SDL_Event& e);

      // \Name: handleKeyDown
      // \Description:
      // - handles an SDL_KEYDOWN event 
      // \Argument:
      // - const SDL_Event&, the event to handle 
      // \Returns
      // - void
      void handleKeyDown(const SDL_Event& e);

      // \Name: draw
      // \Description:
      // - performs a draw action 
      // \Argument:
      // - DrawType_e&, the type of draw action to perform
      // \Returns
      // - void
      void draw(const DrawType_e& draw_action);

      // \Name: draw
      // \Description:
      // - draws a move or undo_move action
      // \Argument:
      // - const std::pair<int, int>&, the position to draw the moved piece
      // - const std::pair<int, int>&, the position to redraw as a base tile
      // \Returns
      // - void
      void drawMove(const std::pair<int, int>& move_position, const std::pair<int, int>& empty_position);

      // \Name: getTileBaseTexture
      // \Description:
      // - gets the base texture of a particular screen tile
      // \Argument:
      // - const std::pair<int, int>&, the screen tile position
      // \Returns
      // - SDL_Texture*, pointer to the desired texture
      void StandoffApp_c::renderTileBaseSprite(const std::pair<int, int>& tile_position);

      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // the current displayed texture
      SDL_Texture* gTexture = NULL;

      // bulk loads assets and manages them for the client
      ResourceManager_n::ResourceManager_c& mResourceManager;

      // the current game instance
      Game_n::Game_c* mCurrentGame = NULL;
   };
}

#endif _STANDOFF_APP_HPP_
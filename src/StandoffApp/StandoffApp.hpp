#ifndef _STANDOFF_APP_HPP_
#define _STANDOFF_APP_HPP_

// #include <SDL.h>
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
      StandoffApp_c(ResourceManager_n::ResourceManager_c& resource_manager, SDL_Renderer* renderer);

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
      // - none
      // \Returns
      // - void
      void draw();

      // bulk loads assets and manages them for the client
      ResourceManager_n::ResourceManager_c& mResourceManager;

      // the ResourceManager's window renderer
      SDL_Renderer* gRenderer = NULL;

      // the current game instance
      Game_n::Game_c mCurrentGame;

      // let's the main loop know when to wait for a rotation on a deployed piece
      // bool mDeploymentFlag = false;
   };
}

#endif _STANDOFF_APP_HPP_
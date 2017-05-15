#ifndef _STANDOFF_APP_HPP_
#define _STANDOFF_APP_HPP_

#include "../ConnectHandler/ConnectHandler.hpp"
#include "../Game/Game.hpp"
#include "../ResourceManager/ResourceManager.hpp"

namespace StandoffApp_n
{
   enum Mode_e
   {
      LOCAL = 0,
      PLAYER_ONE = 1,
      PLAYER_TWO = 2
   };

   class StandoffApp_c
   {
   public:
      StandoffApp_c(ResourceManager_n::ResourceManager_c& resource_manager, 
                    ConnectHandler_n::ConnectHandler_c& connect_handler,
                    ConnectHandler_n::Address_c& server_address,
                    SDL_Renderer* renderer, Mode_e mode);

      ~StandoffApp_c();

      int run();

   protected:

      // handles an SDL_MOUSEBUTTONDOWN event (only for SDL_BUTTON_LEFT)
      void handleLmbDown(const SDL_Event& e);

      // handles an SDL_KEYDOWN event 
      void handleKeyDown(const SDL_Event& e);

      // performs a draw action 
      void draw();

      // formats and sends an action message to the game server
      bool update();

      // receives and translates action messages from the game server
      void listen();

      // gets the container of currently accessible pieces
      // const std::vector<Game_n::PiecePtr>& getMyPieces();

      // only called in networked games, true if the current app has the turn priority
      bool myTurn();

      // bulk loads assets and manages them for the client
      ResourceManager_n::ResourceManager_c& mResourceManager;

      // handles message transmission between client and server for networked games
      ConnectHandler_n::ConnectHandler_c& mConnectHandler;

      // simplify communication with server by storing its address
      ConnectHandler_n::Address_c& mServerAddress;

      // the ResourceManager's window renderer
      SDL_Renderer* gRenderer;

      // the current game instance
      Game_n::Game_c mCurrentGame;

      // indicates whether the game is local or networked, always defined
      Mode_e mMode;
   };
}

#endif

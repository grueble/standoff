#ifndef _STANDOFF_SERVER_HPP_
#define _STANDOFF_SERVER_HPP_

#include "../ConnectHandler/ConnectHandler.hpp"
#include "../Game/Game.hpp"
#include <vector>

namespace StandoffServer_n
{
   struct Game_s
   {
      Game_s(int game_id, ConnectHandler_n::Address_c p1_addr) :
         mGameId(game_id),
         mP1Addr(p1_addr),
         mP2Addr(),
         mGame(),
         mIsOpen(true)
      {
         
      }

      int mGameId;
      ConnectHandler_n::Address_c mP1Addr;
      ConnectHandler_n::Address_c mP2Addr;
      Game_n::Game_c mGame;
      bool mIsOpen;
      std::string mName;
   };

   // wrap Game_s to allow for automatic deletion
   typedef std::unique_ptr<Game_s> GamePtr;

   class StandoffServer_c
   {
   public:
      StandoffServer_c(ConnectHandler_n::ConnectHandler_c& connect_handler);

      ~StandoffServer_c();

      int run();

   private:
      // private constant used in StandoffServer_c, defined in StandoffServer.cpp
      static const int MAX_NUM_GAMES;

      // sends a list of games to the client
      void findGames(ConnectHandler_n::Address_c& to_address);

      // handles message transmission between client and server
      ConnectHandler_n::ConnectHandler_c& mConnectHandler;

      // list of active games
      std::vector<GamePtr> mGames;
   };
}

#endif

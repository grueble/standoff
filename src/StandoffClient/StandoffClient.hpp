#ifndef _STANDOFF_CLIENT_HPP_
#define _STANDOFF_CLIENT_HPP_
#include "../ConnectHandler/ConnectHandler.hpp"
#include "StandoffApp.hpp"
#include <cstdio>

const int SERVER_PORT = 27270;
const int GAME_NAME_LENGTH_MAX = 32;

namespace StandoffClient_n
{
   class StandoffClient_c
   {
   public:
      // constructor
      StandoffClient_c(ConnectHandler_n::ConnectHandler_c& connect_handler); 

      // default destructor
      ~StandoffClient_c();

      int run();

private:
      // launches the Standoff
      void playApp(StandoffApp_n::Mode_e mode);

      // prints a line break to the console (formatting)
      void printBreak() { printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"); }

      // handles message transmission between client and server
      ConnectHandler_n::ConnectHandler_c& mConnectHandler;

      // container of the ID's of every open games
      std::vector<int> mOpenGames;
      
      // simplify communication with server by storing its address
      ConnectHandler_n::Address_c mServerAddress;
   };
}

#endif 

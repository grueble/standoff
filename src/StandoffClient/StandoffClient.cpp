#include "StandoffClient.hpp"
#include <string>
#include <iostream>
#include <cstdio>
#include <sstream>

using namespace StandoffClient_n;

// need to undef main in order to block SDL from hijacking the console
#undef main
int main(int argc, char* argv[])
{
   ConnectHandler_n::ConnectHandler_c connect_handler;
   StandoffClient_c standoff_client(connect_handler);
   return standoff_client.run();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Constructor and Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StandoffClient_c::StandoffClient_c(ConnectHandler_n::ConnectHandler_c& connect_handler) :
   mConnectHandler(connect_handler),
   mServerAddress(ConnectHandler_n::Address_c(127,0,0,1,ConnectHandler_n::ConnectHandler_c::SERVER_PORT))
{

}

StandoffClient_c::~StandoffClient_c()
{

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - run
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int StandoffClient_c::run()
{
   // platform specific library initialization
   if (!mConnectHandler.initializeSockets())
   {
      printf("Failed to initialize sockets!\n");
      return 1;
   }

   // store input from getline
   std::string input = "";

   // data is serialized into this buffer prior to sending
   unsigned char data[ConnectHandler_n::MAX_PACKET_SIZE] = {0};

   // open a client socket
   std::cout << "Enter a unique port ( > 1024 ) for your client application: " << std::endl;
   getline(std::cin, input);
   mConnectHandler.start(std::stoi(input)); 

   // while (true)
   // {
   //    data[0] = 6;
   //    mConnectHandler.sendData(mServerAddress, data);
   // }

   while (!(input.at(0) == 'Q' || input.at(0) == 'q'))
   { 
      std::cout << "(C)reate a new game" << std::endl;
      std::cout << "(D)isplay a list of active games" << std::endl;
      std::cout << "(J)oin an existing game" << std::endl;
      std::cout << "(Q)uit" << std::endl;
      getline(std::cin, input);

      switch(input.at(0))
      {
         case 'C' :
         case 'c' :
         {
            std::cout << "Creating a new game..." << std::endl;
            std::cout << "(L)ocal" << std::endl;
            std::cout << "(N)etworked" << std::endl;
            getline(std::cin, input);
            switch(input.at(0))
            {
               case 'L' :
               case 'l' :
               {
                  playApp(StandoffApp_n::LOCAL);
                  break;
               }
               case 'N' :
               case 'n' :
               {
                  data[0] = 0x01; // ConnectHandler_n::CREATE_GAME 
                  if (!mConnectHandler.sendData(mServerAddress, data))
                  {    
                     printf("Message failed to send to server!\n");
                  }
                  else 
                  {
                     bool receive_complete = false;
                     while (!receive_complete)
                     {
                        if (mConnectHandler.receiveData(mServerAddress, data) > 0)
                        {
                           receive_complete = true;
                           playApp(StandoffApp_n::PLAYER_ONE);
                        }
                     }
                  }
                  break;
               }
               default:
               {
                  std::cout << "INVALID input, try again..." << std::endl;
                  break;
               }
            }
            break;
         }
         case 'D' :
         case 'd' :
         {
            std::cout << "Displaying a list of active games..." << std::endl;
            data[0] = 0x02; // ConnectHandler_n::FIND_GAMES 
            if (!mConnectHandler.sendData(mServerAddress, data))
            {   
               printf("Message failed to send to server!\n");
            }
            else 
            {
               int receive_count = 0;
               bool receive_complete = false;
               while (!receive_complete)
               {
                  if (mConnectHandler.receiveData(mServerAddress, data) > 0)
                  {
                     receive_count++;
                     std::cout << data[1] << " | " << std::endl; // mGameId
                     if (data[2] == 1) // misOpen = true
                     {
                        std::cout << "O | " << std::endl; // indicates an open game
                        mOpenGames.push_back(data[1]);
                     }
                     else 
                     {
                        std::cout << "X | " << std::endl; // indicates a closed game
                     }
                     // iteratively print the game's name
                     for (int i = 0; i < data[3]; ++i)
                     {
                        std::cout << data[4 + i] << std::endl;
                     } 
                     if (receive_count >= data[0])
                     {
                        receive_complete = true;
                     }
                  }
               }
            }
            break;
         }
         case 'J' :
         case 'j' :
         {
            std::cout << "Enter the ID of the game you want to join: ";
            getline(std::cin, input);
            int game_id = std::stoi(input);
            std::vector<int>::iterator it;
            for (it = mOpenGames.begin(); it != mOpenGames.end(); ++it)
            {
               if (*it == game_id)
               {
                  std::cout << "Joining game #" << game_id << "..." << std::endl;
                  data[0] = 0x01; // ConnectHandler_n::JOIN_GAME
                  data[1] = game_id;
                  if (!mConnectHandler.sendData(mServerAddress, data))
                  {   
                     printf("Message failed to send to server!\n");
                  }
                  else 
                  {
                     playApp(StandoffApp_n::PLAYER_TWO);
                  }
                  break;
               }
            }
            printf("Please enter the ID of an open game.");
            break;
         }
         case 'Q' :
         case 'q' :
         {
            std::cout << "Quitting Standoff client application..." << std::endl;
            break;
         }
         default:
         {
            std::cout << "INVALID input, try again..." << std::endl;
            break;
         }
      }
   }

   mConnectHandler.shutdownSockets();

   return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - playApp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void StandoffClient_c::playApp(StandoffApp_n::Mode_e mode)
{
   ResourceManager_n::ResourceManager_c resource_manager;
   if (!resource_manager.init())
   {
      printf("Failed to initialize application!\n");
   }
   else
   {
      if (!resource_manager.loadMedia())
      {
         printf("Failed to load media!\n");
      }
      else 
      {
         StandoffApp_n::StandoffApp_c standoff_app(resource_manager, 
                                                   mConnectHandler,
                                                   mServerAddress,
                                                   resource_manager.getRenderer(),
                                                   mode);
         int exit_reason = standoff_app.run();

         printf("Exited with code %d !", exit_reason);
      }
   }
   resource_manager.close();
}

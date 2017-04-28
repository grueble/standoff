#include "StandoffServer.hpp"
#include <iostream>

using namespace StandoffServer_n;

const int StandoffServer_c::MAX_NUM_GAMES = 255;

int main(int argc, char* argv[])
{
   ConnectHandler_n::ConnectHandler_c connect_handler;
   StandoffServer_c standoff_server(connect_handler);
   return standoff_server.run();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Constructor and Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StandoffServer_c::StandoffServer_c(ConnectHandler_n::ConnectHandler_c& connect_handler) :
   mConnectHandler(connect_handler)
{

}

StandoffServer_c::~StandoffServer_c()
{
   // delete games and players
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - run
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int StandoffServer_c::run()
{
   // platform specific library initialization
   if (!mConnectHandler.initializeSockets())
   {
      printf("Failed to initialize sockets!\n");
      return 1;
   }

   mConnectHandler.start(ConnectHandler_n::ConnectHandler_c::SERVER_PORT);
   
   int game_count = 0;
   bool send_flag = false;
   ConnectHandler_n::Address_c from_address = ConnectHandler_n::Address_c();
   ConnectHandler_n::Address_c to_address = ConnectHandler_n::Address_c();
   unsigned char data[ConnectHandler_n::MAX_PACKET_SIZE];

   while (true) // listen until cancelled by ctrl^C
   {
      if (mConnectHandler.receiveData(from_address, data) > 0)
      {
         switch(static_cast<ConnectHandler_n::Request_e>(data[0])) // skip the cast for perf. improvements
         {
            case ConnectHandler_n::CREATE_GAME :
            {
               if (game_count < MAX_NUM_GAMES)
               {
                  game_count++; 
                  mGames.push_back(std::make_unique<Game_s>(game_count, from_address));
               }
               else 
               {
                  printf("Unable to create game, server @ capacity!\n");
               }
               break;
            }
            case ConnectHandler_n::FIND_GAMES :
            {
               findGames(from_address);
               break;
            }
            case ConnectHandler_n::JOIN_GAME :
            {
               int game_id = data[1];
               std::vector<GamePtr>::iterator it;
               for (it = mGames.begin(); it != mGames.end(); ++it)
               {
                  if ((*it)->mGameId == game_id)
                  {
                     (*it)->mP2Addr = from_address;
                     to_address = (*it)->mP1Addr;
                     send_flag = true;
                     break;
                  }
               }
               break;
            }
            case ConnectHandler_n::MOVE_ACTION :
            {
               std::pair<int, int> start_position = std::make_pair(data[1], data[2]);
               std::pair<int, int> move_position = std::make_pair(data[2], data[3]);
               std::vector<GamePtr>::iterator it;
               for (it = mGames.begin(); it != mGames.end(); ++it)
               {
                  if ((*it)->mP1Addr == from_address)
                  {
                     const std::vector<Game_n::PiecePtr>& p1_pieces = (*it)->mGame.getPlayer1Pieces();
                     std::vector<Game_n::PiecePtr>::const_iterator p1_it;
                     for (p1_it = p1_pieces.begin(); p1_it != p1_pieces.end(); ++p1_it)
                     {
                        /*
                         * we only move a piece if it's current position is the
                         * same as the moved piece's previous position...
                         */
                        if ((*p1_it)->getPosition() == start_position)
                        {
                           // indicates a move or deploy action
                           if (start_position != move_position)
                           {
                              (*p1_it)->setPosition(move_position);
                           }

                           // unnecessary changes in direction are caught by Piece_c
                           (*p1_it)->setDirection(static_cast<Piece_n::Direction_e>(data[5]));
                        }
                     }
                     to_address = (*it)->mP2Addr;
                     send_flag = true;
                     break;
                  }
                  else if ((*it)->mP2Addr == from_address)
                  {
                     const std::vector<Game_n::PiecePtr>& p2_pieces = (*it)->mGame.getPlayer2Pieces();
                     std::vector<Game_n::PiecePtr>::const_iterator p2_it;
                     for (p2_it = p2_pieces.begin(); p2_it != p2_pieces.end(); ++p2_it)
                     {
                        /*
                         * we only move a piece if it's current position is the
                         * same as the moved piece's previous position...
                         */
                        if ((*p2_it)->getPosition() == start_position)
                        {
                           // indicates a move or deploy action
                           if (start_position != move_position)
                           {
                              (*p2_it)->setPosition(move_position);
                           }

                           // unnecessary changes in direction are caught by Piece_c
                           (*p2_it)->setDirection(static_cast<Piece_n::Direction_e>(data[5]));
                        }
                     }
                     to_address = (*it)->mP1Addr;
                     send_flag = true;
                     break;
                  }
               }
               break;
            }
            case ConnectHandler_n::SHOOTOUT :
            {
               std::vector<GamePtr>::iterator it;
               for (it = mGames.begin(); it != mGames.end(); ++it)
               {
                  if ((*it)->mP1Addr == from_address)
                  {
                     (*it)->mGame.shootout();

                     to_address = (*it)->mP2Addr;
                     send_flag = true;
                     break;
                  }
                  else if ((*it)->mP2Addr == from_address)
                  {
                     (*it)->mGame.shootout();

                     to_address = (*it)->mP1Addr;
                     send_flag = true;
                     break;
                  }
               }
               break;
            }
            default : // ConnectHandler_n::INVALID
            {
               printf("Received invalid request ");
               break;
            }
         }

         if (send_flag)
         {
            if (!mConnectHandler.sendData(to_address, data))
            { 
               printf("Message failed to send to client!\n");
            }
         }
      }
   }

   mConnectHandler.shutdownSockets();

   return 0;
}

void StandoffServer_c::findGames(ConnectHandler_n::Address_c& to_address)
{
   unsigned char data[ConnectHandler_n::MAX_PACKET_SIZE]; 
   data[0] = (unsigned char)(mGames.size());
   std::vector<GamePtr>::iterator it;
   for (it = mGames.begin(); it != mGames.end(); ++it)
   {
      data[1] = (*it)->mGameId;
      data[2] = (*it)->mIsOpen;
      data[3] = (unsigned char)((*it)->mName.length());
      for (unsigned int i = 0; i < (*it)->mName.length(); ++it)
      {
         data[4 + i] = (*it)->mName.at(i);
      }
      if (!mConnectHandler.sendData(to_address, data))
      {
         printf("Message failed to send to client!\n");
      }
   }
}

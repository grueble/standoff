#ifndef _CONNECT_HANDLER_HPP_
#define _CONNECT_HANDLER_HPP_

#include "LSocket.hpp"
#include <vector>
#include <map>
#include <stack>

namespace ConnectHandler_n
{
   const int MAX_PACKET_SIZE = 256;

   enum Request_e
   {
      INVALID = 0,
      CREATE_GAME = 1,
      FIND_GAMES = 2,
      JOIN_GAME = 3,
      MOVE_ACTION = 4,
      SHOOTOUT = 5
   };

   class ConnectHandler_c
   {
   public:
      // public constant used in ConnectHandler_c, defined in ConnectHandler.cpp
      static const int SERVER_PORT;

      ConnectHandler_c();
      
      virtual ~ConnectHandler_c();

      bool initializeSockets();

      void shutdownSockets();
      
      bool start(int port);
      
      void stop();
      
      bool isRunning() const;
      
      virtual bool sendData(Address_c& to_address, unsigned char data[]);
      
      virtual int receiveData(Address_c& from_address, unsigned char data[]);
         
   private:
      // private constants used in ConnectHandler_c, defined in ConnectHandler.cpp
      static const int PROTOCOL_ID;
      static const int HEADER_SIZE;

      unsigned int mProtocolId;
      
      bool mIsRunning;
   
      LSocket mSocket;
   };
}

#endif
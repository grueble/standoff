#include "ConnectHandler.hpp"
#include <cstring>

using namespace ConnectHandler_n;

const int ConnectHandler_c::SERVER_PORT = 27270;
const int ConnectHandler_c::PROTOCOL_ID = 0x99887766;
const int ConnectHandler_c::HEADER_SIZE = 4;

ConnectHandler_c::ConnectHandler_c()
{
   mProtocolId = PROTOCOL_ID;
   mIsRunning = false;
}

ConnectHandler_c::~ConnectHandler_c()
{
   if (isRunning())
   {
      stop();
   }
}

bool ConnectHandler_c::initializeSockets()
{
#if PLATFORM == PLATFORM_WINDOWS 
   WSADATA WsaData;
   return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
   return true;
#endif
}

void ConnectHandler_c::shutdownSockets()
{
#if PLATFORM == PLATFORM_WINDOWS
   WSACleanup();
#endif
}

bool ConnectHandler_c::start(int port)
{
   assert(!mIsRunning);
   if (!mSocket.open(port))
   {
      return false;
   }
   mIsRunning = true;
   return true;
}

void ConnectHandler_c::stop()
{
   assert(mIsRunning);
   mSocket.closeSocket();
   mIsRunning = false;
}

bool ConnectHandler_c::isRunning() const
{
   return mIsRunning;
}

bool ConnectHandler_c::sendData(Address_c& to_address, unsigned char data[])
{
   assert(mIsRunning);
   if (to_address.getAddress() == 0)
   {
      return false;
   }

   printf("Sending to %d.%d.%d.%d:%d\n", 
            to_address.getA(), to_address.getB(), 
            to_address.getC(), to_address.getD(), 
            to_address.getPort());

   unsigned char* packet = new unsigned char[MAX_PACKET_SIZE + HEADER_SIZE];
   packet[0] = (unsigned char)(mProtocolId >> 24);
   packet[1] = (unsigned char)((mProtocolId >> 16) & 0xFF);
   packet[2] = (unsigned char)((mProtocolId >> 8) & 0xFF);
   packet[3] = (unsigned char)((mProtocolId) & 0xFF);

   // printf("%d, %d, %d, %d\n", packet[0], packet[1], packet[2], packet[3]);

   std::memcpy(&packet[HEADER_SIZE], data, MAX_PACKET_SIZE);
   bool ret_val = 
      mSocket.send(to_address, packet, MAX_PACKET_SIZE + HEADER_SIZE);
   delete [] packet;
   return ret_val;
}

int ConnectHandler_c::receiveData(Address_c& from_address, unsigned char data[])
{
   // attempt to read the message
   assert(mIsRunning);
   unsigned char* packet = new unsigned char[MAX_PACKET_SIZE + HEADER_SIZE];
   int bytes_read = 
      mSocket.receive(from_address, packet, MAX_PACKET_SIZE + HEADER_SIZE);

   if (bytes_read > 0)
   {
      printf("%d bytes read...\n", bytes_read);
   }

   // determine if the message is valid
   if (bytes_read == 0)
   {
      delete [] packet;
      return 0;
   }
   if (bytes_read <= HEADER_SIZE)
   {
      delete [] packet;
      return 0;
   }

   // printf("%d, %d, %d, %d\n", packet[0], packet[1], packet[2], packet[3]);

   if (packet[0] != (unsigned char)(mProtocolId >> 24) || 
       packet[1] != (unsigned char)((mProtocolId >> 16) & 0xFF) ||
       packet[2] != (unsigned char)((mProtocolId >> 8) & 0xFF) ||
       packet[3] != (unsigned char)(mProtocolId & 0xFF))
   {
      printf("FUCK");
      delete [] packet;
      return 0;
   }

   printf("Receiving from %d.%d.%d.%d:%d\n", 
            from_address.getA(), from_address.getB(), 
            from_address.getC(), from_address.getD(), 
            from_address.getPort());

   std::memcpy(data, &packet[HEADER_SIZE], MAX_PACKET_SIZE);
   delete [] packet;
   return bytes_read - HEADER_SIZE;
}

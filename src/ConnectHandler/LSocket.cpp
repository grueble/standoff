#include "LSocket.hpp"
#include <cstdio>

using namespace ConnectHandler_n;

LSocket::LSocket()
{

}

LSocket::~LSocket()
{

}

bool LSocket::open(unsigned short port)
{
   // attempt to assign (RAII) socket handle
   mSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

   if (this->mSocket <= 0)
   {
      printf("Failed to create socket!\n");
      closeSocket();
      return false;
   }

   sockaddr_in address;
   address.sin_family = PF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons((unsigned short)port);

   if (bind(mSocket, (const sockaddr*)&address, sizeof(sockaddr_in)) == NET_SOCKET_ERROR)
   {
      printf("Failed to bind socket!\n");
      closeSocket();
      return false;
   }
   printf("Listening on port %d...\n", port);

   // platform specific function call to set socket to non-blocking IO
   int result(NET_SOCKET_ERROR);
   #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX 
      int flags = fcntl(mSocket, F_GETFL, 0);
      flags |= O_NONBLOCK;                    
      result = fcntl(mSocket, F_SETFL, flags);
   #elif PLATFORM == PLATFORM_WINDOWS
      unsigned long nonBlocking(1);
      result = ioctlsocket(mSocket,FIONBIO,&nonBlocking);
   #endif

   if (result == NET_SOCKET_ERROR)
   {
      printf("Failed to set non-blocking socket!\n" );
      closeSocket();
   }
   return isOpen();
}

void LSocket::closeSocket()
{
   if (isOpen())
   {
      // platform specific function call to close socket
      #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
         close(this->mSocket);
      #elif PLATFORM == PLATFORM_WINDOWS
         closesocket(this->mSocket);
      #endif
      
      mSocket = NET_INVALID_SOCKET;
   }
}

bool LSocket::isOpen() const
{
   return mSocket != NET_INVALID_SOCKET;
}

bool LSocket::send(const Address_c & destination,
                   const void* packet_data,
                   int packet_size)
{
   assert(packet_data);
   assert(packet_size > 0);

   if (!isOpen()) 
   { 
      return false; 
   }

   sockaddr_in addr;
   addr.sin_family = PF_INET;
   addr.sin_addr.s_addr = htonl(destination.getAddress());
   addr.sin_port = htons(destination.getPort());

   int sent_bytes = 
      sendto(mSocket, (const char*)packet_data, packet_size, 0, (sockaddr*)&addr, sizeof(sockaddr_in));

   if (sent_bytes != packet_size)
   {
       printf( "failed to send packet\n" );
       return false;
   }

   return true;
}


int LSocket::receive(ConnectHandler_n::Address_c &sender,
                     void* packet_data,
                     int max_packet_size)
{
   assert(packet_data);
   assert(max_packet_size > 0);
      
   if (!isOpen())
   {
      return false;
   }

   #if PLATFORM == PLATFORM_WINDOWS
      typedef int socklen_t;
   #endif

   sockaddr_in from;
   socklen_t fromLength = sizeof(from);

   int received_bytes = 
      recvfrom(mSocket, (char*)packet_data, max_packet_size, 0, (sockaddr*)&from, &fromLength);

   // NOTE: these two conditions occur in different end states (fail vs. shutdown)
   if (received_bytes == NET_SOCKET_ERROR || received_bytes == 0)
   {
      return 0;
   }

   unsigned int from_address = ntohl(from.sin_addr.s_addr);
   unsigned int from_port = ntohs(from.sin_port);

   sender = Address_c(from_address, from_port);

   return received_bytes;
}

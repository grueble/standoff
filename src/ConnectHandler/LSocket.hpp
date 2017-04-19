#ifndef _L_SOCKET_HPP_
#define _L_SOCKET_HPP_

// platform definitions
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

// detect platform
#if defined(_WIN32)
   #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
   #define PLATFORM PLATFORM_MAC
#else
   #define PLATFORM PLATFORM_UNIX
#endif

// platform specific includes
#if PLATFORM == PLATFORM_WINDOWS
   #include <winsock2.h>

   // this sidesteps linking from project/makefile
   #pragma comment( lib, "wsock32.lib" )
   #pragma warning( disable : 4996  )

   namespace ConnectHandler_n
   {
      // platform specific type definition
      typedef SOCKET Socket_t;
      const Socket_t NET_INVALID_SOCKET = INVALID_SOCKET;

      // platform specific error enumeration
      enum SocketErrors_e
      {
         NET_SOCKET_ERROR = SOCKET_ERROR
      };
   }
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <fcntl.h>
   #include <unistd.h>

   namespace ConnectHandler_n
   {
      // platform specific type definition
      typedef int Socket_t;

      // platform specific error enumeration
      enum SocketErrors_e
      {
         NET_SOCKET_ERROR = -1, 
         NET_INVALID_SOCKET = -1
      };
   }
#else
   #error Unknown platform!
#endif

#include <assert.h>
#include "Address.hpp"

namespace ConnectHandler_n
{
   class LSocket
   {
   public:
      LSocket();

      ~LSocket();

      bool open(unsigned short port);
    
      void closeSocket();
    
      bool isOpen() const;
    
      bool send(const Address_c& destination, const void* data, int size);
    
      int receive(Address_c& sender, void* data, int size);
   private:
      Socket_t mSocket;
   };
}

#endif
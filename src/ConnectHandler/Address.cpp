#include "Address.hpp"

using namespace ConnectHandler_n;

Address_c::Address_c()
{
   init(0, 0);
}

Address_c::~Address_c()
{

}

Address_c::Address_c(unsigned char a,
                     unsigned char b,
                     unsigned char c,
                     unsigned char d,
                     unsigned short port)
{
   unsigned int address = (a << 24) | (b << 16) | (c << 8) | d;
   init(address, port);
}

Address_c::Address_c(unsigned int address,
                     unsigned short port )
{
   init(address, port);
}

void Address_c::init(unsigned int address, unsigned short port)
{
   mAddress = address;
   mPort = port;
}

unsigned int Address_c::getAddress() const
{
   return mAddress;
}

unsigned short Address_c::getPort() const
{
   return mPort;
}

unsigned char Address_c::getA() const
{
   return (unsigned char)(mAddress >> 24);
}

unsigned char Address_c::getB() const
{
   return (unsigned char)(mAddress >> 16);
}

unsigned char Address_c::getC() const
{
   return (unsigned char)(mAddress >> 8);
}

unsigned char Address_c::getD() const
{
   return (unsigned char)(mAddress);   
}
#ifndef _ADDRESS_HPP_
#define _ADDRESS_HPP_

namespace ConnectHandler_n
{
    class Address_c
    {
    public:
        Address_c();
        
        ~Address_c();
        
        Address_c(unsigned char a, unsigned char b,
                  unsigned char c, unsigned char d,
                  unsigned short port );

        Address_c(unsigned int address, unsigned short port);
        
        unsigned int getAddress() const;
        
        unsigned short getPort() const;
    
        unsigned char getA() const;
        
        unsigned char getB() const;
        
        unsigned char getC() const;
        
        unsigned char getD() const;

        bool operator==(const Address_c &other) const
        {
           return this->mAddress == other.mAddress && this->mPort == other.mPort;
        }
        
        bool operator!=(const Address_c &other) const
        {
           return !(*this == other);
        }
    
    private:
        void init(unsigned int address, unsigned short port);

        unsigned int mAddress;

        unsigned short mPort;
    };
}

#endif

#ifndef ADDRESS_H
#define ADDRESS_H

class Address {
public:
    Address()
    {
        address = 0;
        port = 0;
    }

    Address( unsigned int address, unsigned short port )
    {
        this->address = address;
        this->port = port;
    }

    unsigned int GetAddress() const
    {
        return address;
    }

    unsigned short GetPort() const
    {
        return port;
    }

    bool operator == ( const Address & other ) const
    {
        return address == other.address && port == other.port;
    }

    bool operator != ( const Address & other ) const
    {
        return ! ( *this == other );
    }

    bool operator < ( const Address & other ) const
    {
        // note: this is so we can use address as a key in std::map
        if ( address < other.address )
            return true;
        if ( address > other.address )
            return false;
        else
            return port < other.port;
    }

private:
    unsigned int address;
    unsigned short port;
};

#endif // ADDRESS_H

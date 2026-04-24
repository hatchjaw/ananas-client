#include "Listener.h"

namespace ananas::network {
    MulticastListenerSocket::MulticastListenerSocket(const SocketParams &p)
        : ip(p.ip),
          port(p.port)
    {
    }

    void MulticastListenerSocket::connect()
    {
        socket.beginMulticast(ip, port);
    }

    //==========================================================================

    UnicastListenerSocket::UnicastListenerSocket(const SocketParams &p)
        : port(p.port)
    {
    }

    void UnicastListenerSocket::connect()
    {
        socket.begin(port);
    }
}

#ifndef ANANASLISTENER_H
#define ANANASLISTENER_H

#include <NetworkProcessor.h>
#include <NetworkUtils.h>

namespace ananas::network
{
    class MulticastListenerSocket : public NetworkProcessor
    {
    public:
        explicit MulticastListenerSocket(const SocketParams &p);

        void connect() override;

    private:
        IPAddress ip;
        uint16_t port;
    };

    //==========================================================================

    class UnicastListenerSocket : public NetworkProcessor
    {
    public:
        explicit UnicastListenerSocket(const SocketParams &p);

        void connect() override;

    private:
        uint16_t port;
    };
}

#endif //ANANASLISTENER_H

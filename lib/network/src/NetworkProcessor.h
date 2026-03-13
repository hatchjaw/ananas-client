#ifndef NETWORKPROCESSOR_H
#define NETWORKPROCESSOR_H

#include <QNEthernet.h>

namespace ananas::network {

class NetworkProcessor
{
public:
    virtual ~NetworkProcessor() = default;

    virtual void connect() = 0;
protected:
    qindesign::network::EthernetUDP socket{8};
};

}

#endif //NETWORKPROCESSOR_H

#ifndef VIRTUALSOURCECOORDINATELISTENER_H
#define VIRTUALSOURCECOORDINATELISTENER_H

#include <AnanasClient.h>

#include "WFSControlContext.h"
#include <OSCBundle.h>
#include <ProgramComponent.h>

namespace ananas::WFS
{
    class VirtualSourceCoordinateListener final : public MulticastListenerSocket,
                                      public ProgramComponent
    {
    public:
        explicit VirtualSourceCoordinateListener(ControlContext &controlContext);

    protected:
        void beginImpl() override;

    public:
        void run() override;

        size_t printTo(Print &p) const override;

    private:
        void parseVirtualSourcePosition(OSCMessage &msg, int addrOffset) const;

        /**
        * Max message size is ~44 bytes; leave some headroom.
        */
        uint8_t rxBuffer[64]{};
        OSCBundle bundleIn;
        OSCMessage messageIn;
        ControlContext &context;
    };
} // ananas::WFS

#endif //VIRTUALSOURCECOORDINATELISTENER_H

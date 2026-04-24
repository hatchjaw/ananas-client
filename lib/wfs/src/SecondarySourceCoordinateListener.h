#ifndef SECONDARYSOURCECOORDINATELISTENER_H
#define SECONDARYSOURCECOORDINATELISTENER_H

#include <Listener.h>
#include <OSCBundle.h>
#include <ProgramComponent.h>
#include <WFSControlContext.h>

namespace ananas::WFS
{
    class SecondarySourceCoordinateListener : public UnicastListenerSocket,
                                              public ProgramComponent
    {
    public:
        explicit SecondarySourceCoordinateListener(ControlContext &controlContext);

    protected:
        void beginImpl() override;

    public:
        void run() override;

        size_t printTo(Print &p) const override;

    private:
        void parseSecondarySourcePosition(OSCMessage &msg, int addrOffset) const;

        uint8_t rxBuffer[128]{};
        OSCBundle bundleIn;
        OSCMessage messageIn;
        ControlContext &context;
    };
}

#endif //SECONDARYSOURCECOORDINATELISTENER_H

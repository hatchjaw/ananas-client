#ifndef CONTROLDATALISTENER_H
#define CONTROLDATALISTENER_H

#include <AnanasClient.h>
#include "AmbisonicsControlContext.h"
#include <OSCBundle.h>
#include <ProgramComponent.h>

namespace ananas::HOA
{
    class ControlDataListener final : public UnicastListenerSocket,
                                      public ProgramComponent
    {
    public:
        explicit ControlDataListener(ControlContext &controlContext);

    protected:
        void beginImpl() override;

    public:
        void run() override;

        size_t printTo(Print &p) const override;

    private:
        void parseCoefficient(OSCMessage &msg, int addrOffset) const;

        void parseRow(OSCMessage &msg, int addrOffset) const;

        uint8_t rxBuffer[MTU]{};
        OSCBundle bundleIn;
        OSCMessage messageIn;
        ControlContext &context;
    };
} // ananas::WFS

#endif //CONTROLDATALISTENER_H

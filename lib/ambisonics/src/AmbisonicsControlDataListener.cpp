#include "AmbisonicsControlDataListener.h"
#include <AnanasUtils.h>
#include <QNEthernet.h>

namespace ananas::Ambisonics
{
    ControlDataListener::ControlDataListener(ControlContext &controlContext)
        : ListenerSocket(Constants::AmbisonicsControlSocketParams),
          context(controlContext)
    {
    }

    void ControlDataListener::beginImpl()
    {
    }

    void ControlDataListener::run()
    {
        if (const auto size{socket.parsePacket()}; size > 0) {
            socket.read(rxBuffer, size);

            // Try to read as bundle
            bundleIn.empty();
            bundleIn.fill(rxBuffer, size);
            if (!bundleIn.hasError() && bundleIn.size() > 0) {
                bundleIn.route("/module", [this](OSCMessage &msg, int addrOffset) { parseModule(msg, addrOffset); });
            } else {
                // Try as message
                messageIn.empty();
                messageIn.fill(rxBuffer, size);
                if (!messageIn.hasError() && messageIn.size() > 0) {
                    messageIn.route("/module", [this](OSCMessage &msg, int addrOffset) { parseModule(msg, addrOffset); });
                }
            }
        }
    }

    size_t ControlDataListener::printTo(Print &p) const
    {
        return 0;
    }

    void ControlDataListener::parseModule(OSCMessage &msg, int addrOffset) const
    {
        char ipString[16];
        IPAddress ip;
        msg.getString(0, ipString, 16);
        ip.fromString(ipString);
        if (ip == qindesign::network::Ethernet.localIP()) {
            char id[2];
            msg.getAddress(id, addrOffset + 1);
            const auto numericID{strtof(id, nullptr)};
            // Serial.printf("Receiving module ID: %f\n", numericID);
            context.moduleID = numericID;
        }
    }
}

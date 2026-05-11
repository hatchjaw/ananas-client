#include "VirtualSourceCoordinateListener.h"

#include <AnanasUtils.h>
#include <QNEthernet.h>

namespace ananas::WFS
{
    VirtualSourceCoordinateListener::VirtualSourceCoordinateListener(ControlContext &controlContext)
        : MulticastListenerSocket(Constants::VirtualSourceControlSocketParams),
          context(controlContext)
    {
    }

    void VirtualSourceCoordinateListener::beginImpl()
    {
    }

    void VirtualSourceCoordinateListener::run()
    {
        if (const auto size{socket.parsePacket()}; size > 0) {
            socket.read(rxBuffer, size);

            // Try to read as bundle
            bundleIn.empty();
            bundleIn.fill(rxBuffer, size);
            if (!bundleIn.hasError() && bundleIn.size() > 0) {
                bundleIn.route("/vs", [this](OSCMessage &msg, int addrOffset) { parseVirtualSourcePosition(msg, addrOffset); });
            } else {
                // Try as message
                messageIn.empty();
                messageIn.fill(rxBuffer, size);
                if (!messageIn.hasError() && messageIn.size() > 0) {
                    messageIn.route("/vs", [this](OSCMessage &msg, int addrOffset) { parseVirtualSourcePosition(msg, addrOffset); });
                }
            }
        }
    }

    size_t VirtualSourceCoordinateListener::printTo(Print &p) const
    {
        return 0;
    }

    void VirtualSourceCoordinateListener::parseVirtualSourcePosition(OSCMessage &msg, int addrOffset) const
    {
        // Get the virtual source index and coordinate axis, e.g. "vs/0/x"
        char path[20];
        msg.getAddress(path, 1);
        // Get the coordinate value.
        const auto pos{msg.getFloat(0)};
        // Serial.printf("Receiving \"%s\": %f\n", path, pos);
        // Set the parameter.
        auto it{context.virtualSourceCoordinates.find(path)};
        if (it != context.virtualSourceCoordinates.end()) {
            it->second.set(pos);
        } else {
            Serial.printf("%s not found\n", path);
        }
    }
}

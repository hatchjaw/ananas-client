#include "SecondarySourceCoordinateListener.h"

namespace ananas::WFS
{
    SecondarySourceCoordinateListener::SecondarySourceCoordinateListener(ControlContext &controlContext)
        : UnicastListenerSocket(Constants::SecondarySourceControlSocketParams),
          context(controlContext)
    {
    }

    void SecondarySourceCoordinateListener::beginImpl()
    {
    }

    void SecondarySourceCoordinateListener::run()
    {
        if (const auto size{socket.parsePacket()}; size > 0) {
            socket.read(rxBuffer, size);

            // Try to read as bundle
            bundleIn.empty();
            bundleIn.fill(rxBuffer, size);
            if (!bundleIn.hasError() && bundleIn.size() > 0) {
                bundleIn.route("/ss", [this](OSCMessage &msg, int addrOffset) { parseSecondarySourcePosition(msg, addrOffset); });
            } else {
                // Try as message
                messageIn.empty();
                messageIn.fill(rxBuffer, size);
                if (!messageIn.hasError() && messageIn.size() > 0) {
                    messageIn.route("/ss", [this](OSCMessage &msg, int addrOffset) { parseSecondarySourcePosition(msg, addrOffset); });
                }
            }
        }
    }

    size_t SecondarySourceCoordinateListener::printTo(Print &p) const
    {
        return 0;
    }

    void SecondarySourceCoordinateListener::parseSecondarySourcePosition(OSCMessage &msg, int addrOffset) const
    {
        // Get the virtual source index and coordinate axis, e.g. "ss/0/x"
        char path[20];
        msg.getAddress(path, 1);
        // Get the coordinate value.
        const auto pos{msg.getFloat(0)};
        // Serial.printf("Receiving \"%s\": %f\n", path, pos);
        // Set the parameter.
        auto it{context.secondarySourceCoordinates.find(path)};
        if (it != context.secondarySourceCoordinates.end()) {
            it->second = pos;
        } else {
            Serial.printf("%s not found\n", path);
        }
    }
}

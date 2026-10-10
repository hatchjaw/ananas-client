#include "AmbisonicsControlDataListener.h"
#include <AnanasUtils.h>
#include <QNEthernet.h>

namespace ananas::HOA
{
    ControlDataListener::ControlDataListener(ControlContext &controlContext)
        : UnicastListenerSocket(Constants::AmbisonicsControlSocketParams),
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
                bundleIn.route("/dm", [this](OSCMessage &msg, int addrOffset) { parseCoefficient(msg, addrOffset); });
                bundleIn.route("/row", [this](OSCMessage &msg, int addrOffset) { parseRow(msg, addrOffset); });
            } else {
                // Try as message
                messageIn.empty();
                messageIn.fill(rxBuffer, size);
                if (!messageIn.hasError() && messageIn.size() > 0) {
                    messageIn.route("/dm", [this](OSCMessage &msg, int addrOffset) { parseCoefficient(msg, addrOffset); });
                    messageIn.route("/row", [this](OSCMessage &msg, int addrOffset) { parseRow(msg, addrOffset); });
                }
            }
        }
    }

    size_t ControlDataListener::printTo(Print &p) const
    {
        return 0;
    }

    void ControlDataListener::parseCoefficient(OSCMessage &msg, int addrOffset) const
    {
        // Get the decoder matrix row and index within that row, e.g. "dm/0/0"
        char path[20];
        msg.getAddress(path, 1);
        // Get the coefficient value.
        const auto pos{msg.getFloat(0)};
        // Serial.printf("Receiving \"%s\": %f\n", path, pos);
        // Set the parameter.
        auto it{context.decoderCoefficients.find(path)};
        if (it != context.decoderCoefficients.end()) {
            it->second = pos;
        } else {
            Serial.printf("%s not found\n", path);
        }
    }

    void ControlDataListener::parseRow(OSCMessage &msg, int addrOffset) const
    {
        char path[1];
        msg.getAddress(path, addrOffset + 1);
        const int idx{path[0] - '0'};
        const auto row{msg.getInt(0)};

        switch (idx) {
            case 0:
                context.decoderRow0 = row;
                break;
            case 1:
                context.decoderRow1 = row;
                break;
            default:
                break;
        }
    }
}

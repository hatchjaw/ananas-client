#ifndef ANANASPACKET_H
#define ANANASPACKET_H

#include "AnanasUtils.h"
#include <SystemUtils.h>

namespace ananas
{
    struct Packet
    {
        uint8_t *rawData() { return reinterpret_cast<uint8_t *>(this); }
    };

    struct AudioPacket : Packet
    {
#pragma pack(push, 1)
        struct Header
        {
            uint16_t sequenceNumber;
            NanoTime time;
            uint8_t numChannels;
            uint16_t numFrames;
        };
#pragma pack(pop)

        int16_t *audio() { return reinterpret_cast<int16_t *>(data); }

        Header header{};
        uint8_t data[MTU - sizeof(Header)]{};
    };

#pragma pack(push, 1)
    struct ClientAnnouncePacket : Packet
    {
        const uint32_t serial{SystemUtils::computeSerialNumber()};
        SystemUtils::FirmwareType firmwareType;
        SystemUtils::VersionNumber firmwareVersion;
        float samplingRate;
        float percentCPU;
        int32_t presentationOffsetFrame;
        NanoTime presentationOffsetNs;
        int32_t audioPtpOffsetNs;
        uint8_t bufferFillPercent;
        bool ptpLock;
        float secondarySource0x{0.f};
        float secondarySource0y{0.f};
        float secondarySource1x{0.f};
        float secondarySource1y{0.f};
    };
#pragma pack(pop)

    struct AuthorityAnnouncePacket : Packet
    {
        const uint32_t serial{SystemUtils::computeSerialNumber()};
        uint32_t usbFeedbackAccumulator;
        int numUnderruns;
        int numOverflows;
    };
}

#endif //ANANASPACKET_H

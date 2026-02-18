#ifndef ANANASNETWORKUTILS_H
#define ANANASNETWORKUTILS_H

#include <Arduino.h>
#include <IPAddress.h>
#include <TimeLib.h>

namespace ananas::network
{
    using NanoTime = int64_t;

    struct SocketParams
    {
        IPAddress ip;
        uint16_t port{};
    };

    struct AnnounceSocketParams : SocketParams
    {
        uint intervalMs{};
    };

    enum PTPMessageType : uint8_t
    {
        Sync = 0x0,
        DelayReq = 0x1,
        FollowUp = 0x8,
        DelayResp = 0x9,
        Announce = 0xB
    };

    struct PTPMessage
    {
        uint16_t size;
        PTPMessageType type;
        uint8_t control;
    };

#pragma pack(push, 1)

    struct PTPV2PacketHeader
    {
        uint8_t messageType{0};
        uint8_t versionPTP{2};
        uint16_t messageLength{0};
        uint8_t domainNumber{0};
        uint8_t reserved1{0};
        uint8_t flagField[2]{};
        int64_t correctionField{0};
        int32_t reserved2{0};
        uint8_t sourcePortIdentity[10]{};
        uint16_t sequenceID{0};
        uint8_t controlField{0};
        uint8_t logMessageInterval{0x7f};
    };

    struct PTPV2Packet
    {
        PTPV2PacketHeader header;
        uint8_t originTimestamp[10]{};
    };

    struct PTPV2AnnouncePacket
    {
        PTPV2PacketHeader header;
        uint8_t timestamp[10]{};
        int16_t currentUTCOffset{37};
        uint8_t reserved3{0};
        uint8_t gmPriority1{128};
        uint32_t gmClockQuality{0xFFFFFEF8};
        uint8_t gmPriority12{128};
        uint8_t gmIdentity[8]{};
        uint8_t stepsRemoved[2]{};
        uint8_t timeSource{0xa0};
    };

    struct PTPV2DelayResponsePacket
    {
        PTPV2PacketHeader header;
        uint8_t receiveTimestamp[10]{};
        uint8_t requestingPortIdentity[10]{};
    };

#pragma pack(pop)

    class Constants
    {
    public:
        inline static const SocketParams PTPEventSocketParams{
            {224, 0, 1, 129},
            319
        };

        inline static const SocketParams PTPGeneralSocketParams{
            {224, 0, 1, 129},
            320
        };

        static constexpr PTPMessage AnnounceMessage{
            64,
            Announce,
            5
        };

        static constexpr PTPMessage SyncMessage{
            44,
            Sync,
            0
        };

        static constexpr PTPMessage FollowUpMessage{
            44,
            FollowUp,
            2
        };

        static constexpr PTPMessage DelayRequestMessage{
            44,
            DelayReq,
            1
        };

        static constexpr PTPMessage DelayResponseMessage{
            54,
            DelayResp,
            3
        };

        static constexpr int HardwareOffsetNs{-200};
        static constexpr NanoTime NanosecondsPerSecond{1'000'000'000};
    };

    class Utils
    {
    public:
        static NanoTime timespecToNanoTime(const timespec &ts)
        {
            return ts.tv_sec * Constants::NanosecondsPerSecond + ts.tv_nsec;
        }

        static void nanoTimeToTimespec(const NanoTime t, timespec &ts)
        {
            ts.tv_sec = t / Constants::NanosecondsPerSecond;
            ts.tv_nsec = t % Constants::NanosecondsPerSecond;
        }

        template<size_t N>
        static void writeBigEndian(uint8_t *dest, const NanoTime value)
        {
            for (size_t i{0}; i < N; i++) {
                dest[i] = value >> (N - 1 - i) * 8 & 0xff;
            }
        }

        template<size_t N>
        static NanoTime readBigEndian(const uint8_t *src)
        {
            NanoTime value = 0;
            for (size_t i = 0; i < N; i++) {
                value = (value << 8) | src[i];
            }
            return value;
        }

        static void printTime(const NanoTime t)
        {
            NanoTime x = t;
            const int ns = x % 1000;
            x /= 1000;
            const int us = x % 1000;
            x /= 1000;
            const int ms = x % 1000;
            x /= 1000;

            tmElements_t tme;
            breakTime(x, tme);

            Serial.printf("%02d.%02d.%04d %02d:%02d:%02d::%03d:%03d:%03d\n", tme.Day, tme.Month, 1970 + tme.Year, tme.Hour, tme.Minute, tme.Second, ms, us, ns);
        }

        static void printTime(const timespec &t)
        {
            printTime(timespecToNanoTime(t));
        }
    };
}

#endif //ANANASNETWORKUTILS_H
